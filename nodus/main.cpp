#include <iostream>
#include <gp_Pnt.hxx>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        try {
            auto request = json::parse(line);

            std::string method = request["method"];
            json params = request["params"];
            json result;

            if (method == "processJson") {
                // Handle logic
                result = { "status", "ok", "received", params };
            }

            json response = {
                {"jsonrpc", "2.0"},
                {"result", result},
                {"id", request["id"]}
            };

            std::cout << response.dump() << std::endl;
            std::cout.flush();
        } catch (const std::exception& e) {
            json error = {
                {"jsonrpc", "2.0"},
                {"error", {{"code", -32603}, {"message", e.what()}}},
                {"id", nullptr}
            };
            std::cout << error.dump() << std::endl;
            std::cout.flush();
        }
    }
}