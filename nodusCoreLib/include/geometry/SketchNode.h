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

    class SketchNode : public Node
    {
    private:
        // members
        gp_Ax3 _origin;

    public:
        struct SketchWithOrigin
        {
            gp_Ax3 origin; // XY of the origin axis is always XY on the sketch. to change, send in a different axis

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(SketchWithOrigin, origin);
        };

    public:
        SketchNode(nlohmann::json params, std::shared_ptr<Node> parent);

        TopoDS_Shape Build() override;

        virtual gp_Ax3 GetOrigin() override;
    };

};

#endif
