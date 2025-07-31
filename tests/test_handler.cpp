#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"

using namespace rpc;

TEST_CASE("handle_request correctly processes valid JSON-RPC request") {
    std::string valid_request = R"({
        "jsonrpc": "2.0",
        "method": "render",
        "params": { "foo": 123, "bar": "hello" },
        "id": 42
    })";

    auto response = handle_request(valid_request);

    auto id = response["id"];
    auto dumped = response.dump();

    CHECK(response["jsonrpc"] == "2.0");
    CHECK(response["id"] == 42);
    CHECK(response.contains("result"));

    auto result = response["result"];
    CHECK(result["status"] == "ok");
    CHECK(result["echo"]["foo"] == 123);
    CHECK(result["echo"]["bar"] == "hello");
}

TEST_CASE("handle_request returns error for unknown method") {
    std::string bad_method_request = R"({
        "jsonrpc": "2.0",
        "method": "unknownMethod",
        "params": {},
        "id": 100
    })";

    auto response = handle_request(bad_method_request);

    CHECK(response["jsonrpc"] == "2.0");
    CHECK(response["id"].is_null()); // error returns null id
    CHECK(response.contains("error"));

    auto error = response["error"];
    CHECK(error["code"] == -32603);
    CHECK(error["message"].get<std::string>().find("Unknown method") != std::string::npos);
}

TEST_CASE("handle_request returns error for invalid JSON") {
    std::string invalid_json = R"({ this is not valid json })";

    auto response = handle_request(invalid_json);

    CHECK(response["jsonrpc"] == "2.0");
    CHECK(response["id"].is_null());
    CHECK(response.contains("error"));

    auto error = response["error"];
    CHECK(error["code"] == -32603);
    CHECK(error["message"].get<std::string>().length() > 0);
}