#include "geometry/LineNode.h"

#include <BRepBuilderAPI_MakeEdge.hxx>

namespace nodus::core::geometry
{

    LineNode::LineNode(nlohmann::json &params) : Node()
    {
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
    int LineNode::Build()
    {
        return 1;
    }

}