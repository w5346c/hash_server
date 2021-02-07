#include <iostream>
#include <thread>

#include <boost/program_options.hpp>
#include <boost/asio/io_context.hpp>

#include "server.h"

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

        boost::asio::io_context context;
        Server server(context, port);

        auto threadCount = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

        for(int i = 0; i < threadCount; i++)
            threads.emplace_back([&context]{ context.run(); });

        for(auto& thread : threads)
            thread.join();
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

