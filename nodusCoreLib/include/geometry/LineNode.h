#ifndef LineNode_H
#define LineNode_H

#include "geometry/Node.h"
#include "nlohmann/json.hpp"
// #include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include "primatives/gp_conversion.h"

using namespace nlohmann;

namespace nodus::core::geometry
{

    class LineNode : public Node<int>
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

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(LineParamsStartEnd, start, end);
        };

    public:
        LineNode(nlohmann::json &params);
        LineNode(LineParamsStartEnd &params);

        int Build();
    };

};

#endif
