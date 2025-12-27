#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"
#include "protocol/jsonrpc.h"
#include "transport/mock_transport.h"
#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <StlAPI_Writer.hxx>
#include <TopoDS.hxx>
#include <iostream>
#include "Model.h"
#include "geometry/LineNode.h"
#include "geometry/SketchNode.h"
#include "geometry/ExtrudeNode.h"
using namespace JSONRPC;
using namespace nodus::core;

TEST_CASE("Nodus Test creating line from jsons")
{

    auto extrude = std::make_shared<geometry::ExtrudeNode>(
        json{{"id", "extrude"}, {"length", 2}}, nullptr);

    json j = {
        {"id", "sketch"},
        {"origin", {
                       {"location", {0.0, 0.0, 0.0}}, // Array for gp_Pnt
                       {"direction", {0.0, 0.0, 1.0}} // Array for gp_Vec
                   }}};

    auto sketch = std::make_shared<geometry::SketchNode>(
        j,
        extrude);

    extrude->SetChildren({sketch});

    auto line1 = std::make_shared<geometry::LineNode>(
        json{{"id", "l1"}, {"start", {0.0, 0.0}}, {"end", {2.0, 0.0}}},
        sketch);
    auto line2 = std::make_shared<geometry::LineNode>(
        json{{"id", "l2"}, {"start", {2.0, 0.0}}, {"end", {2.0, 2.0}}},
        sketch);
    auto line3 = std::make_shared<geometry::LineNode>(
        json{{"id", "l3"}, {"start", {2.0, 2.0}}, {"end", {0.0, 2.0}}},
        sketch);
    auto line4 = std::make_shared<geometry::LineNode>(
        json{{"id", "l4"}, {"start", {0.0, 2.0}}, {"end", {0.0, 0.0}}},
        sketch);

    sketch->SetChildren({line1,
                         line2,
                         line3,
                         line4});

    auto shape = extrude->Build();

    TopoDS_Compound cp = TopoDS::Compound(shape);

    BRepCheck_Analyzer analyzer(shape);
    if (!analyzer.IsValid())
    {
        std::cout << "Shape is not valid!" << std::endl;
        // Don't write invalid shapes to STEP
        return;
    }

    // STEP format (recommended)
    Interface_Static::SetCVal("write.step.schema", "AP203");
    STEPControl_Writer stepWriter;
    IFSelect_ReturnStatus status = stepWriter.Transfer(cp, STEPControl_ManifoldSolidBrep);
    if (status != IFSelect_RetDone)
    {
        throw std::runtime_error("Failed to transfer shape to STEP writer");
    }

    std::string outputPath = "C:\\Users\\grant\\Documents\\output3.step";
    status = stepWriter.Write(outputPath.c_str());
    if (status != IFSelect_RetDone)
    {
        throw std::runtime_error("Failed to write STEP file");
    }

    BRepMesh_IncrementalMesh mesh(shape, 0.1);
    mesh.Perform();

    StlAPI_Writer stlWriter;
    std::string outputPathstl = "C:\\Users\\grant\\Documents\\cube_test.stl";
    stlWriter.Write(shape, outputPathstl.c_str());

    std::cout << "STEP file written to: " << outputPath << std::endl;
}

TEST_CASE("Nodus correctly processes valid extrusion")
{
    std::string valid_request = R"({
        "type": "extrude",
        "props": {
            "id": "extrude1",
            "length": 20.0
        },
        "children": [
            {
            "type": "sketch",
            "props": {
                "id": "sketch1",
                "origin": {
                    "location":[0,0,0],
                    "direction":[0,0,1]
                    }
            },
            "children": [
                {
                "type": "line",
                "props": { "id": "line1","start": [0, 0], "end": [20, 0] },
                "children": []
                },
                {
                "type": "line",
                "props": { "id": "line2","start": [20, 0], "end": [20, 20] },
                "children": []
                },
                {
                "type": "line",
                "props": { "id": "line3","start": [20, 20], "end": [0, 20] },
                "children": []
                },
                {
                "type": "line",
                "props": { "id": "line4","start": [0, 20], "end": [0, 0] },
                "children": []
                }
            ]
            },
            {
            "type": "sketch",
            "props": {
                "id": "sketch1",
                "origin": {
                    "location":[0,45,0],
                    "direction":[0,0,1]
                    }
            },
            "children": [
                {
                "type": "line",
                "props": { "id": "line1","start": [0, 0], "end": [20, 0] },
                "children": []
                },
                {
                "type": "line",
                "props": { "id": "line2","start": [20, 0], "end": [20, 20] },
                "children": []
                },
                {
                "type": "line",
                "props": { "id": "line3","start": [20, 20], "end": [0, 20] },
                "children": []
                },
                {
                "type": "line",
                "props": { "id": "line4","start": [0, 20], "end": [0, 0] },
                "children": []
                }
            ]
            }
        ]
        })";

    auto model = Model(json::parse(valid_request));
    auto shape = model.Render();

    BRepCheck_Analyzer analyzer(shape);
    if (!analyzer.IsValid())
    {
        std::cout << "Shape is not valid!" << std::endl;
        // Don't write invalid shapes to STEP
        return;
    }

    TopTools_IndexedMapOfShape vertices, edges, faces, solids, shells, wires;
    TopExp::MapShapes(shape, TopAbs_VERTEX, vertices);
    TopExp::MapShapes(shape, TopAbs_EDGE, edges);
    TopExp::MapShapes(shape, TopAbs_FACE, faces);
    TopExp::MapShapes(shape, TopAbs_SOLID, solids);
    TopExp::MapShapes(shape, TopAbs_SHELL, shells);
    TopExp::MapShapes(shape, TopAbs_WIRE, wires);

    // STEP format (recommended)
    Interface_Static::SetCVal("write.step.schema", "AP203");
    STEPControl_Writer stepWriter;
    IFSelect_ReturnStatus status = stepWriter.Transfer(shape, STEPControl_ManifoldSolidBrep);
    if (status != IFSelect_RetDone)
    {
        throw std::runtime_error("Failed to transfer shape to STEP writer");
    }

    std::string outputPath = "C:\\Users\\grant\\Documents\\modelTest.step";
    status = stepWriter.Write(outputPath.c_str());
    if (status != IFSelect_RetDone)
    {
        throw std::runtime_error("Failed to write STEP file");
    }

    BRepMesh_IncrementalMesh mesh(shape, 0.1);
    mesh.Perform();

    StlAPI_Writer stlWriter;
    std::string outputPathstl = "C:\\Users\\grant\\Documents\\modelTest.stl";
    stlWriter.Write(shape, outputPathstl.c_str());

    std::cout << "STEP file written to: " << outputPath << std::endl;
}
