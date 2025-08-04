#include "geometry/LineNode.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "gp_Pnt.hxx"

namespace nodus::core::geometry
{

    TopoDS_Shape LineNode::Build()
    {
        auto [success1, startEndParams] = try_convert_json<LineNode::LineParamsStartEnd>(_params);
        auto [success2, start2dEndParams] = try_convert_json<LineNode::LineParams2dStartEnd>(_params);
        if (success1)
        {
            return Build(startEndParams);
        }
        else if (success2)
        {
            return Build(start2dEndParams);
        }
        else
        {
            throw std::invalid_argument("JSON does not match any known LineNode parameter structure");
        }
    }
    TopoDS_Shape LineNode::Build(LineParamsStartEnd &params)
    {
        BRepBuilderAPI_MakeEdge edgeBuilder(params.start, params.end);

        if (!edgeBuilder.IsDone())
        {
            throw std::runtime_error("Failed to create line edge from start and end points");
        }

        return edgeBuilder.Edge();
    }

    TopoDS_Shape LineNode::Build(LineParams2dStartEnd &params)
    {
        auto origin = GetOrigin();

        auto getPoint = [origin](std::pair<double, double> pt)
        {
            gp_Vec xdir = origin.XDirection();
            gp_Vec ydir = origin.YDirection();

            gp_Vec scaledXVec = xdir * pt.first;
            gp_Vec scaledYVec = ydir * pt.second;

            auto combined = scaledYVec + scaledXVec;
            auto point = gp_Pnt(combined.X(), combined.Y(), combined.Z());
            return point;
        };

        LineParamsStartEnd newparams;
        newparams.start = getPoint(params.start);
        newparams.end = getPoint(params.end);
        return Build(newparams);
    }
}
