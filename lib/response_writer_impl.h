#pragma once

#include "response_writer.h"

namespace hash_server {

class ResponseWriterImpl : public ResponseWriter
{
public:
    boost::system::error_code WriteResponse(
        boost::asio::ip::tcp::socket& socket,
        const std::string& response) override;
};

} // namespace hash_server