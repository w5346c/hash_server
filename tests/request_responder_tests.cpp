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

using namespace ::testing;

namespace hash_server::tests {

struct DummySocket : boost::asio::socket_base
{};

class RequestResponderTest : public ::testing::Test {
protected:

    RequestResponderTest()
    {
        auto hashCalculator = std::make_shared<NiceMock<MockHashCalculator>>();
        m_responseWriter = std::make_shared<MockResponseWriter>();

        ON_CALL(*hashCalculator, CalculateHashString(_)).WillByDefault(Invoke(
            [](const std::string& s)
            {
                return std::string("hash(") + s + ")";
            }));

        m_testee = CreateRequestResponder(hashCalculator, m_responseWriter);
    }

    std::shared_ptr<MockResponseWriter> m_responseWriter;

    std::shared_ptr<RequestResponder> m_testee;

};

TEST_F(RequestResponderTest, CompleteRequest)
{
    DummySocket dummySocket;

    // TODO: dirty crutch; there must be a better way...
    auto& socket = reinterpret_cast<boost::asio::ip::tcp::socket&>(dummySocket);

    InSequence seq;
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String01)!"));
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String02)!"));

    EXPECT_FALSE(m_testee->ProcessData(socket, "String01!String02!"));
}

TEST_F(RequestResponderTest, IncompleteRequest)
{
    DummySocket dummySocket;
    auto& socket = reinterpret_cast<boost::asio::ip::tcp::socket&>(dummySocket);

    InSequence seq;
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, _)).Times(0);

    EXPECT_FALSE(m_testee->ProcessData(socket, "RequestWithNoNewLine"));
}

TEST_F(RequestResponderTest, PartialRequests)
{
    DummySocket dummySocket;
    auto& socket = reinterpret_cast<boost::asio::ip::tcp::socket&>(dummySocket);

    InSequence seq;
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String01)!"));
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String02)!"));
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String03)!"));
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String04)!"));
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(String05)!"));

    EXPECT_FALSE(m_testee->ProcessData(socket, "Str"));
    EXPECT_FALSE(m_testee->ProcessData(socket, "ing01!String02!String03!Str"));
    EXPECT_FALSE(m_testee->ProcessData(socket, "ing04!S"));
    EXPECT_FALSE(m_testee->ProcessData(socket, "trin"));
    EXPECT_FALSE(m_testee->ProcessData(socket, "g05!"));
}

TEST_F(RequestResponderTest, AllStringsEmpty)
{
    DummySocket dummySocket;
    auto& socket = reinterpret_cast<boost::asio::ip::tcp::socket&>(dummySocket);

    InSequence seq;
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash()!")).Times(5);

    EXPECT_FALSE(m_testee->ProcessData(socket, "!!!!!"));
}

TEST_F(RequestResponderTest, SomeStringsEmpty)
{
    DummySocket dummySocket;
    auto& socket = reinterpret_cast<boost::asio::ip::tcp::socket&>(dummySocket);

    InSequence seq;
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(S)!"));
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash()!")).Times(3);
    EXPECT_CALL(*m_responseWriter, WriteResponse(_, "hash(P)!"));

    EXPECT_FALSE(m_testee->ProcessData(socket, "S!!!!P!"));
}

} // namespace hash_server::tests