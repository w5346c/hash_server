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
{
    assert(hashCalculator);
    assert(responseWriter);
}

boost::system::error_code RequestResponderImpl::ProcessData(
    boost::asio::ip::tcp::socket& socket,
    const std::string& request)
{
    try
    {
        auto len = request.size();

        size_t stringStartIdx{};

        for (size_t i = 0; i < len; i++)
        {
            if (request[i] == '!')
            {
                auto response = m_hashCalculator->CalculateHashString(
                    m_unprocessedData + request.substr(stringStartIdx, i - stringStartIdx)) + "!";

                m_unprocessedData.clear();

                EXPECT_OK(m_responseWriter->WriteResponse(socket, response));

                stringStartIdx = i + 1;
            }
        }

        if (stringStartIdx < len)
        {
            m_unprocessedData.append(request.substr(stringStartIdx, len - stringStartIdx));
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