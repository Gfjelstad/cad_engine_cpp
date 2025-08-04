#ifndef ExtrudeNode_H
#define ExtrudeNode_H

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

    class ExtrudeNode : public Node
    {
    private:
        // members
    public:
        // members
        struct ExtrudeParamsLength
        {
            double length;
            // int start;
            // int end;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(ExtrudeParamsLength, length);
        };

    public:
        using Node::Node;

        TopoDS_Shape Build() override;
        TopoDS_Shape Build(ExtrudeParamsLength &params);
    };

};

#endif
