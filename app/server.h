#pragma once

#include <optional>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <hash_calculator.h>
#include <response_writer.h>

namespace hash_server {

class Server
{
public:

    explicit Server(boost::asio::io_context& io_context, std::uint16_t port);

    void Accept();

private:

    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::optional<boost::asio::ip::tcp::socket> m_socket;

    std::shared_ptr<HashCalculator> m_hashCalculator;
    std::shared_ptr<ResponseWriter> m_responseWriter;
};

} // namespace hash_server