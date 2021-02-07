#pragma once

#include <memory>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace hash_server {

class ResponseWriter
{
public:
    virtual boost::system::error_code WriteResponse(
        boost::asio::ip::tcp::socket& socket,
        const std::string& response) = 0;
};

std::shared_ptr<ResponseWriter> CreateResponseWriter();

} // namespace hash_server