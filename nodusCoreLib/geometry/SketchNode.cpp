#include "geometry/SketchNode.h"
// #include "SketchNode.h"
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

namespace nodus::core::geometry
{
    SketchNode::SketchNode(nlohmann::json params, std::shared_ptr<Node> parent) : Node(params, parent)
    {
        auto [success1, sketchWithOrigin] = try_convert_json<SketchNode::SketchWithOrigin>(_params);
        if (success1)
        {
            _origin = sketchWithOrigin.origin;
        }
        else
        {
            _origin = GetOrigin();
        }
    }

    TopoDS_Shape SketchNode::Build()
    {
        auto wireBuilder = BRepBuilderAPI_MakeWire();

        for (auto &childNode : _children)
        {
            auto shape = childNode->Build();
            wireBuilder.Add(TopoDS::Edge(shape));
        }
        wireBuilder.Check();
        if (!wireBuilder.IsDone())
        {
            throw std::runtime_error("wirebuilder failed");
        }
        auto faceBuilder = BRepBuilderAPI_MakeFace(wireBuilder.Wire(), true);
        if (!faceBuilder.IsDone())
        {
            throw std::runtime_error("faceBuilder failed");
        }
        return faceBuilder.Shape();
    }

    gp_Ax3 SketchNode::GetOrigin()
    {
        return _origin;
    }
}
