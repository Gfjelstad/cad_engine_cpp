#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"
#include "protocol/jsonrpc.h"
#include "transport/mock_transport.h"
#include <iostream>
// using namespace com;
using namespace JSONRPC;

TEST_CASE("RPCHandler correctly processes valid JSON-RPC request")
{
    std::string valid_request = R"({
        "jsonrpc": "2.0",
        "method": "render",
        "params": { "foo": 123, "bar": "hello" },
        "id": 42
    })";
    std::vector<std::string> messages = {valid_request};
    std::vector<std::string> output = {};
    std::unique_ptr<ITransport> transport = std::make_unique<MockTransport>(messages, &output);
    auto handler = RPCHandler(std::move(transport));

    class TestRequestHandler
    {
    public:
        nlohmann::json handleRequest(std::string method, nlohmann::json params)
        {
            CHECK(method == "render");
            std::printf(params.dump().c_str());
            nlohmann::json result;
            result["worked"] = true;
            return result;
        }
    };

    auto reqhandler = TestRequestHandler();
    RequestFunctionType h = [&reqhandler](std::string method, const nlohmann::json &params)
    {
        return reqhandler.handleRequest(method, params);
    };

    handler.OnRequestHandler(h);

    handler.Listen();

    CHECK(output.size() == 1);
}

TEST_CASE("JSONRPC::JSONRPCResponse")
{
    JSONRPCResponse p;

    auto result = json();
    result["test"] = 1;

    p.id = 1;
    p.jsonrpc = "2.0";
    p.result = result;

    json j = p;

    auto p2 = j.template get<JSONRPCResponse>();
    CHECK(p2.result["test"] == 1);

    json j2 = p2;

    std::string str = j.dump();
    std::string str2 = j2.dump();

    CHECK(str == str2);
}
TEST_CASE("JSONRPC::JSONRPCError")
{
    JSONRPCError::ErrorType e;
    e.code = "112";
    e.message = "HELLO";

    JSONRPCError p;
    p.id = 1;
    p.jsonrpc = "2.0";
    p.error = e;

    json j = p;

    auto p2 = j.template get<JSONRPCError>();
    CHECK(p2.error.message == p.error.message);
    CHECK(p2.error.code == p.error.code);

    json j2 = p2;

    std::string str = j.dump();
    std::string str2 = j2.dump();

    CHECK(str == str2);
}

TEST_CASE("JSONRPC::JSONRPCRequest")
{
    auto params = json();
    params["test"] = 1;

    JSONRPCRequest p;
    p.id = 1;
    p.jsonrpc = "2.0";
    p.method = "method";
    p.params = params;

    json j = p;
    std::string checkstr = j.dump();

    auto p2 = j.template get<JSONRPCRequest>();
    CHECK(p2.params["test"] == 1);

    json j2 = p2;

    std::string str = j.dump();
    std::string str2 = j2.dump();

    CHECK(str == str2);
}
