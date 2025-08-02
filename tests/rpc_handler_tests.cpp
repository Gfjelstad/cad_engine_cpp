
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"
#include "protocol/jsonrpc.h"
#include "transport/mock_transport.h"
#include <iostream>
using namespace JSONRPC;

TEST_CASE("RPCHandler correctly processes valid JSON-RPC request")
{

    class TestRequestHandler : public RPCHandler
    {
        using RPCHandler::RPCHandler;

    protected:
        virtual nlohmann::json RequestHandler(std::string method, const nlohmann::json &params) override
        {
            CHECK(method == "render");
            std::printf(params.dump().c_str());
            nlohmann::json result;
            result["worked"] = true;
            return result;
        }
    };

    std::string valid_request = R"({
        "jsonrpc": "2.0",
        "method": "render",
        "params": { "foo": 123, "bar": "hello" },
        "id": 42
    })";
    std::vector<std::string> messages = {valid_request};
    auto output = std::make_shared<std::vector<std::string>>();
    std::unique_ptr<ITransport> transport = std::make_unique<MockTransport>(messages, output);

    auto handler = TestRequestHandler(std::move(transport));

    handler.Listen();

    CHECK(output->size() == 1);
    json o = json::parse(output->at(0));
    auto result = o.template get<JSONRPCResponse>();
    CHECK(result.id == 42);
    CHECK(result.result["worked"]);
}
