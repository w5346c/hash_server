#include "request_responder_impl.h"

#include <iostream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#define EXPECT_OK(res) { \
    if (res) throw boost::system::system_error(res); \
}

namespace hash_server {

RequestResponderImpl::RequestResponderImpl(
    std::shared_ptr<HashCalculator> hashCalculator,
    std::shared_ptr<ResponseWriter> responseWriter)
    : m_hashCalculator(hashCalculator)
    , m_responseWriter(responseWriter)
{}

boost::system::error_code RequestResponderImpl::ProcessRequest(
    boost::asio::ip::tcp::socket& socket,
    const std::string& request)
{
    try
    {
        size_t strStart{};

        auto len = request.size();

        for (size_t i = 0; i < len; i++)
        {
            if (request[i] != '\n')
                continue;
    
            auto response = m_hashCalculator->CalculateHashString(
                m_unrespondedData + request.substr(strStart, i - strStart)) + "\n";

            m_unrespondedData.clear();

            EXPECT_OK(m_responseWriter->WriteResponse(socket, response));

            strStart = i + 1;
        }

        if (strStart < len)
        {
            m_unrespondedData.append(request.substr(strStart, len - strStart));
        }
    }
    catch (const boost::system::system_error& e)
    {
        return e.code();
    }

    return boost::system::error_code();
}

std::shared_ptr<RequestResponder> CreateRequestResponder(
    std::shared_ptr<HashCalculator> hashCalculator,
    std::shared_ptr<ResponseWriter> responseWriter)
{
    return std::make_shared<RequestResponderImpl>(
        std::move(hashCalculator),
        std::move(responseWriter));
}

} // namespace hash_server