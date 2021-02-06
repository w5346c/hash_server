#pragma once

#include <memory>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>

class HashCalculator;
class ResponseWriter;

namespace hash_server {

class RequestResponder
{
public:
    virtual boost::system::error_code ProcessData(
        boost::asio::ip::tcp::socket& socket,
        const std::string& request) = 0;
};

std::shared_ptr<RequestResponder> CreateRequestResponder(
    std::shared_ptr<HashCalculator> hashCalculator,
    std::shared_ptr<ResponseWriter> responseWriter);

} // namespace hash_server