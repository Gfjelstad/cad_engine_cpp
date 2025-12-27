#include "geometry/Node.h"
#include "geometry/SketchNode.h"
#include "geometry/ExtrudeNode.h"
#include "geometry/LineNode.h"
#include <iostream>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>
#include <RWGltf_CafWriter.hxx>
#include <TColStd_IndexedDataMapOfStringString.hxx>
#include <Message_ProgressRange.hxx>
#include <XCAFDoc_ShapeTool.hxx>

namespace nodus::core::geometry
{
    std::shared_ptr<Node> CreateNode(const nlohmann::json &node, std::shared_ptr<Node> parent)
    {

        if (!node.contains("type") || !node.contains("props"))
        {
            std::cerr << "node does not contain a valid type" << std::endl;
            throw std::exception();
        };
        std::string type = node.at("type");
        if (type == "sketch")
        {
            return std::make_shared<SketchNode>(node.at("props"), parent);
        }
        if (type == "extrude")
        {
            return std::make_shared<ExtrudeNode>(node.at("props"), parent);
        }
        if (type == "line")
        {
            return std::make_shared<LineNode>(node.at("props"), parent);
        }

        return nullptr;
    }

    std::string Node::Render()
    {
        auto shape = Build();
        // Create a temporary document
        Handle(TDocStd_Document) doc = new TDocStd_Document("BinXCAF");

        // Get the shape tool from the document
        Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

        // Add the shape to the document
        TDF_Label shapeLabel = shapeTool->AddShape(shape);

        // Create a string stream to write to memory
        std::ostringstream memoryStream;

        // Create the GLB writer with the memory stream
        RWGltf_CafWriter writer(memoryStream, true);

        // Perform the export directly to memory
        TColStd_IndexedDataMapOfStringString metadata;
        Message_ProgressRange progress;

        if (writer.Perform(doc, metadata, progress))
        {
            return memoryStream.str();
        }

        return {}; // Return empty string on failure
    }
}