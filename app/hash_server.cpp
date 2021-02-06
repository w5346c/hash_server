#include <iostream>
#include <thread>
#include <utility>
#include <chrono>
#include <atomic>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include <hash_calculator.h>
#include <response_writer.h>
#include <request_responder.h>

#define EXPECT_OK(res) { \
    if (res) throw boost::system::system_error(res); \
}

namespace hash_server {

using tcp = boost::asio::ip::tcp;

void session(tcp::socket socket, std::shared_ptr<hash_server::RequestResponder> responder)
{
    assert(responder);

    std::cout << "session() begin, socket = " << socket.native_handle() << std::endl;

    using namespace std::chrono_literals;

    const int MaxBufferSize = 512;
    char buffer[MaxBufferSize] = {};

    boost::system::error_code res;

    try
    {
        while (true)
        {
            size_t bytesRead = socket.read_some(boost::asio::buffer(buffer), res);

            if (res == boost::asio::error::eof)
                break;
      
            EXPECT_OK(res);

            std::string str(buffer, bytesRead);
            
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end()); // TODO: remove me

            EXPECT_OK(responder->ProcessData(socket, str));
        }

        std::cout << "session() end, socket = " << socket.native_handle() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception in session(): " << e.what() << std::endl;
    }
}

} // namespace hash_server

int main(int argc, char* argv[])
{
    using namespace hash_server;

    try
    {
        uint16_t port{};

        // Process command line arguments
        {
            namespace po = boost::program_options;

            po::options_description desc("Allowed options");
            desc.add_options()
                ("help,h", "produce help message")
                ("port,p", po::value<uint16_t>(&port)->required(), "set the server port");

            po::variables_map varMap;
            po::store(po::parse_command_line(argc, argv, desc), varMap);

            if (varMap.count("help"))
            {
                std::cout << desc << std::endl;
                return false;
            }

            po::notify(varMap);
        }

        boost::asio::io_context ioContext;
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), port));

        auto hashCalculator = CreateHashCalculator();
        auto responseWriter = CreateResponseWriter();

        std::cout << "Hash server started on port " << port << std::endl;
    
        while (true)
        {
            std::thread(
                session,
                acceptor.accept(),
                CreateRequestResponder(hashCalculator, responseWriter)).detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}