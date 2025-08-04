#ifndef LineNode_H
#define LineNode_H

#include "geometry/Node.h"
#include "nlohmann/json.hpp"
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <vector>
// #include "gp_Pnt.hxx"
#include "primatives/gp_conversion.h"

using namespace nlohmann;

namespace nodus::core::geometry
{

    class LineNode : public Node
    {
    private:
        // members
        TopoDS_Edge edge_;

    public:
        // members
        struct LineParamsStartEnd
        {
            gp_Pnt start;
            gp_Pnt end;
            // int start;
            // int end;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(LineParamsStartEnd, start, end);
        };

        struct LineParams2dStartEnd
        {
            std::pair<double, double> start;
            std::pair<double, double> end;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(LineParams2dStartEnd, start, end);
        };

    public:
        using Node::Node;
        // LineNode(nlohmann::json &params);

        TopoDS_Shape Build() override;
        TopoDS_Shape Build(LineParamsStartEnd &params);
        TopoDS_Shape Build(LineParams2dStartEnd &params);
    };

};

#endif
