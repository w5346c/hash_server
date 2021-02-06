#include "response_writer_impl.h"

#include <boost/asio.hpp>

namespace hash_server {

boost::system::error_code ResponseWriterImpl::WriteResponse(
    boost::asio::ip::tcp::socket& socket,
    const std::string& s)
{
    try
    {
        boost::asio::write(socket, boost::asio::buffer(s.c_str(), s.size()));
    }
    catch (const boost::system::system_error& e)
    {
        return e.code();
    }

    return boost::system::error_code();
}

std::shared_ptr<ResponseWriter> CreateResponseWriter()
{
    return std::make_shared<ResponseWriterImpl>();
}

} // namespace hash_server