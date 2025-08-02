#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"
#include "protocol/jsonrpc.h"
#include "transport/mock_transport.h"
#include <iostream>
#include "geometry/LineNode.h"
using namespace JSONRPC;

TEST_CASE("Nodus Test creating line from jsons")
{
    std::string valid_request = R"({
        "start": [0,0,0],
        "end": [1,1,1]
    })";

    json j = json::parse(valid_request);

    auto line = nodus::core::geometry::LineNode(j);

    auto linegeometry = line.Build();
}

TEST_CASE("Nodus correctly processes valid extrusion")
{
    std::string valid_request = R"({
        "jsonrpc": "2.0",
        "method": "render",
        "params": {
        "type": "Extrude",
        "props": {
            "id": "extrude1",
            "direction": [0, 0, 1],
            "distance": 3.0
        },
        "children": [
            {
            "type": "Sketch",
            "props": {
                "plane": "XY"
            },
            "children": [
                {
                "type": "Line",
                "props": { start: [0, 0], end: [2, 0] },
                "children": []
                },
                {
                "type": "Line",
                "props": { start: [2, 0], end: [2, 2] },
                "children": []
                },
                {
                "type": "Line",
                "props": { start: [2, 2], end: [0, 2] },
                "children": []
                },
                {
                "type": "Line",
                "props": { start: [0, 2], end: [0, 0] },
                "children": []
                }
            ]
            }
        ]
        },
        "id": 42
    })";
}