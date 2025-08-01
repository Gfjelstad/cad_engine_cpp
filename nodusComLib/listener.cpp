// #include "include/rpc/handler.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace com
{

    json handle_request(const std::string &request_str)
    {
        try
        {
            auto request = json::parse(request_str);
            std::string method = request["method"];
            json params = request["params"];
            json result;

            if (method == "render")
            {
                result = {
                    {"status", "ok"},
                    {"echo", params}};
            }
            else
            {
                throw std::runtime_error("Unknown method");
            }

            return {
                {"jsonrpc", "2.0"},
                {"result", result},
                {"id", request["id"]}};
        }
        catch (const std::exception &e)
        {
            return {
                {"jsonrpc", "2.0"},
                {"error", {{"code", -32603}, {"message", e.what()}}},
                {"id", nullptr}};
        }
    }
}
