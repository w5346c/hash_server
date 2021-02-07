#pragma once

#include <memory>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

#include <request_responder.h>

namespace hash_server {

class Session : public std::enable_shared_from_this<Session>
{
public:

    explicit Session(
        boost::asio::ip::tcp::socket&& socket,
        std::shared_ptr<RequestResponder> responder);

    void Start();

private:

    void ReadDataAsync();
    void OnDataReceived(boost::system::error_code error, size_t len);

private:

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buf;

    std::shared_ptr<RequestResponder> m_responder;
};

} // namespace hash_server