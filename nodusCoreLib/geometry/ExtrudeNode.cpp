#include "geometry/ExtrudeNode.h"

#include "BRepPrimAPI_MakePrism.hxx"
#include "gp_Pnt.hxx"
#include <TopoDS.hxx>

namespace nodus::core::geometry
{

    TopoDS_Shape ExtrudeNode::Build()
    {
        auto [success1, startEndParams] = try_convert_json<ExtrudeParamsLength>(_params);
        if (success1)
        {
            return Build(startEndParams);
        }
        else
        {
            throw std::invalid_argument("JSON does not match any known LineNode parameter structure");
        }
    }
    TopoDS_Shape ExtrudeNode::Build(ExtrudeParamsLength &params)
    {
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        for (auto &child : _children)
        {
            gp_Vec z = child->GetOrigin().Direction();
            auto extrusionVector = z * params.length;
            auto shape = child->Build();
            TopoDS_Face face = TopoDS::Face(shape);

            auto prismMaker = BRepPrimAPI_MakePrism(face, extrusionVector, false, true);
            builder.Add(compound, prismMaker.Shape());
        }

        TopoDS_Shape shape = compound;

        return shape;
    }

}
