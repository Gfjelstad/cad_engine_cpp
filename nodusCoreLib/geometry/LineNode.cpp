#include "geometry/LineNode.h"

#include "BRepBuilderAPI_MakeEdge.hxx"
#include "gp_Pnt.hxx"

namespace nodus::core::geometry
{

    LineNode::LineNode(nlohmann::json &params)
    {
        auto point = gp_Pnt(1, 1, 1);
        auto [success1, startEndParams] = try_convert_json<LineNode::LineParamsStartEnd>(params);
        if (success1)
        {
            *this = LineNode(startEndParams);
            return;
        }
        else
        {
            throw std::invalid_argument("JSON does not match any known LineNode parameter structure");
        }
    }

    LineNode::LineNode(LineParamsStartEnd &params)
    {
        BRepBuilderAPI_MakeEdge edgeBuilder(params.start, params.end);

        if (!edgeBuilder.IsDone())
        {
            throw std::runtime_error("Failed to create line edge from start and end points");
        }

        edge_ = edgeBuilder.Edge();
    }

    LineNode::LineNode(LineParams2dStartEnd &params)
    {
        auto getPoint = [params](std::pair<double, double> pt)
        {
            gp_Vec scaledXVec = params.xvector * pt.first;
            double xdir[3] = {scaledXVec.X(), scaledXVec.Y(), scaledXVec.Z()};
            gp_Vec scaledYVec = params.yvector * pt.second;

            auto combined = scaledYVec + scaledXVec;
            auto point = gp_Pnt(combined.X(), combined.Y(), combined.Z());
            return point;
        };

        LineParamsStartEnd newparams;
        newparams.start = getPoint(params.start);
        newparams.end = getPoint(params.end);
        *this = LineNode(newparams);
    }

    TopoDS_Shape LineNode::Build()
    {
        return edge_;
    }

}