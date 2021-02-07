#pragma once

#include "request_responder.h"

#include "hash_calculator.h"
#include "response_writer.h"

namespace hash_server {

class RequestResponderImpl : public RequestResponder
{
public:
    explicit RequestResponderImpl(
        std::shared_ptr<HashCalculator> hashCalculator,
        std::shared_ptr<ResponseWriter> responseWriter);

    boost::system::error_code ProcessRequest(
        boost::asio::ip::tcp::socket& socket,
        const std::string& request) override;

private:
    std::shared_ptr<HashCalculator> m_hashCalculator;
    std::shared_ptr<ResponseWriter> m_responseWriter;

    std::string m_unrespondedData;
};

} // namespace hash_server