#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "gp_Pnt.hxx"

#include "geometry/LineNode.h"

using json = nlohmann::json;

int main()
{
    std::cout << "HELLOOOOO" << std::endl;
    std::string valid_request = R"({
        "start": [0,0,0],
        "end": [1,1,1]
    })";

    json j = json::parse(valid_request);

    auto point = gp_Pnt(1, 1, 1);

    auto line = nodus::core::geometry::LineNode(j);

    // auto linegeometry = line.Build();
    // std::string line;
    // while (std::getline(std::cin, line)) {
    //     try {
    //         auto request = json::parse(line);

    //         std::string method = request["method"];
    //         json params = request["params"];
    //         json result;

    //         if (method == "processJson") {
    //             // Handle logic
    //             result = { "status", "ok", "received", params };
    //         }

    //         json response = {
    //             {"jsonrpc", "2.0"},
    //             {"result", result},
    //             {"id", request["id"]}
    //         };

    //         std::cout << response.dump() << std::endl;
    //         std::cout.flush();
    //     } catch (const std::exception& e) {
    //         json error = {
    //             {"jsonrpc", "2.0"},
    //             {"error", {{"code", -32603}, {"message", e.what()}}},
    //             {"id", nullptr}
    //         };
    //         std::cout << error.dump() << std::endl;
    //         std::cout.flush();
    //     }
    // }
}