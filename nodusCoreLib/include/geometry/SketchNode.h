#ifndef SketchNode_H
#define SketchNode_H

#include "geometry/Node.h"
#include "nlohmann/json.hpp"
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
// #include "gp_Pnt.hxx"
#include "primatives/gp_conversion.h"

using namespace nlohmann;

namespace nodus::core::geometry
{

    class SketchNode : public Node<TopoDS_Shape>
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
            gp_Vec xvector;
            gp_Vec yvector;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(LineParams2dStartEnd, start, end, xvector, yvector);
        };

    public:
        SketchNode(nlohmann::json &params, std::vector<TopoDS_Shape> children);
        SketchNode(LineParamsStartEnd &params, std::vector<TopoDS_Shape> children);
        // LineNode(LineParams2dStartEnd &params);

        TopoDS_Shape Build();
    };

};

#endif
