#include "gmock/gmock.h"

#include <hash_calculator.h>
#include <response_writer.h>

using namespace ::testing;

namespace hash_server::tests {

class MockHashCalculator : public HashCalculator
{
public:
    MOCK_METHOD1(CalculateHashString, std::string(const std::string&));
};

class MockResponseWriter : public ResponseWriter
{
public:
    MOCK_METHOD2(WriteResponse, boost::system::error_code(
        boost::asio::ip::tcp::socket&,
        const std::string&));
};

} // namespace hash_server::tests