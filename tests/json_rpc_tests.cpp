#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"
#include "protocol/jsonrpc.h"
#include "transport/mock_transport.h"
#include <iostream>
using namespace JSONRPC;

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
