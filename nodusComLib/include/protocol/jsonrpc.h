#include "nlohmann/json.hpp"
#include <iostream>

using namespace nlohmann;

namespace JSONRPC
{
    struct JSONRPCBase
    {
        std::string id;
        std::string jsonrpc;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(JSONRPCBase, id, jsonrpc);
    };

    struct JSONRPCRequest : JSONRPCBase
    {
        std::string method;
        nlohmann::json params;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(JSONRPCRequest, method, params);
    };
    struct JSONRPCError : JSONRPCBase
    {
        struct ErrorType
        {
            std::string code;
            std::string message;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(ErrorType, code, message);
        };
        ErrorType error;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(JSONRPCError, error);
    };
    struct JSONRPCResponse : JSONRPCBase
    {
        nlohmann::json result;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(JSONRPCResponse, result);
    };
};