#include <iostream>

#include <boost/asio.hpp>

#include "session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

#define EXPECT_OK(res) { \
    if (res) throw boost::system::system_error(res); \
}

namespace hash_server {

Session::Session(tcp::socket&& socket, std::shared_ptr<RequestResponder> responder)
    : m_socket(std::move(socket))
    , m_responder(responder)
{
}

void Session::Start()
{
    std::cout << "Session started, socket: " << m_socket.native_handle() << std::endl;
    ReadDataAsync();
}

void Session::ReadDataAsync()
{
    using namespace std::placeholders;

    try
    {
        io::async_read(m_socket, m_buf,
            io::transfer_at_least(1),
            std::bind(&Session::OnDataReceived, shared_from_this(), _1, _2));
    }
    catch (std::exception& e)
    {
        std::cout << "Session::ReadDataAsync() failed, error: " << e.what() << std::endl;
    }
}

void Session::OnDataReceived(error_code error, size_t len)
{
    using boost::asio::buffers_begin;

    try
    {
        if (error == io::error::eof)
        {
            std::cout << "Session ended, socket: " << m_socket.native_handle() << std::endl;
            return;
        }
            
        EXPECT_OK(error);

        auto bufs = m_buf.data();
        std::string request(buffers_begin(bufs), buffers_begin(bufs) + len);

        m_buf.consume(len);
        EXPECT_OK(m_responder->ProcessRequest(m_socket, request)); // TODO: make it async

        ReadDataAsync();
    }
    catch (std::exception& e)
    {
        std::cout << "Session::OnDataReceived() failed, error: " << e.what() << std::endl;
    }
}

} // namespace hash_server