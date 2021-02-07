#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <hash_calculator.h>
#include <response_writer.h>
#include <request_responder.h>

#include "mocks.h"

namespace hash_server::tests {

using tcp = boost::asio::ip::tcp;

class RequestResponderTest : public ::testing::Test {
protected:

    RequestResponderTest()
    {
        auto hashCalculator = std::make_shared<NiceMock<MockHashCalculator>>();
        m_responseWriter = std::make_shared<StrictMock<MockResponseWriter>>();

        ON_CALL(*hashCalculator, CalculateHashString(_)).WillByDefault(Invoke(
            [](const std::string& s)
            {
                return std::string("hash(") + s + ")";
            }));

        m_testee = CreateRequestResponder(hashCalculator, m_responseWriter);
    }

    boost::asio::ip::tcp::socket& GetDummySocket()
    {   
        // TODO: dirty crutch; there must be a better way...
        return reinterpret_cast<tcp::socket&>(m_dummySocket);
    }

    void CallProcessRequest(const std::string& request)
    {
        EXPECT_FALSE(m_testee->ProcessRequest(GetDummySocket(), request));
    }

    void ExpectResponse(const std::string& response, int times = 1)
    {
        EXPECT_CALL(*m_responseWriter, WriteResponse(_, response)).Times(times);
    }

    void ExpectNoResponse()
    {
        EXPECT_CALL(*m_responseWriter, WriteResponse(_, _)).Times(0);
    }

    std::shared_ptr<MockResponseWriter> m_responseWriter;
    std::shared_ptr<RequestResponder> m_testee;

    char m_dummySocket[sizeof(tcp::socket)] = {};
};

TEST_F(RequestResponderTest, CompleteRequest)
{
    InSequence seq;
    ExpectResponse("hash(String01)\n");
    ExpectResponse("hash(String02)\n");

    CallProcessRequest("String01\nString02\n");
}

TEST_F(RequestResponderTest, IncompleteRequest)
{
    ExpectNoResponse();

    CallProcessRequest("RequestWithNoNewLine");
}

TEST_F(RequestResponderTest, PartialRequests)
{
    InSequence seq;
    ExpectResponse("hash(String01)\n");
    ExpectResponse("hash(String02)\n");
    ExpectResponse("hash(String03)\n");
    ExpectResponse("hash(String04)\n");
    ExpectResponse("hash(String05)\n");

    CallProcessRequest("Str");
    CallProcessRequest("ing01\nString02\nString03\nStr");
    CallProcessRequest("ing04\nS");
    CallProcessRequest("trin");
    CallProcessRequest("g05\n");
}

TEST_F(RequestResponderTest, AllStringsEmpty)
{
    ExpectResponse("hash()\n", 5);

    CallProcessRequest("\n\n\n\n\n");
}

TEST_F(RequestResponderTest, SomeStringsEmpty)
{
    InSequence seq;
    ExpectResponse("hash(S)\n");
    ExpectResponse("hash()\n", 3);
    ExpectResponse("hash(P)\n");

    CallProcessRequest("S\n\n\n\nP\n");
}

} // namespace hash_server::tests