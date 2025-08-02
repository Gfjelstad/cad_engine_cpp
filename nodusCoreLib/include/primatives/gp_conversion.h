#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <nlohmann/json.hpp>

// JSON conversions for OpenCASCADE types

namespace nlohmann
{
    template <>
    struct adl_serializer<gp_Pnt>
    {
        static void to_json(json &j, const gp_Pnt &p)
        {
            j = json{p.X(), p.Y(), p.Z()};
        }

        static void from_json(const json &j, gp_Pnt &p)
        {
            p.SetCoord(j[0].get<double>(), j[1].get<double>(), j[2].get<double>());
        }
    };

    template <>
    struct adl_serializer<gp_Vec>
    {
        static void to_json(json &j, const gp_Vec &v)
        {
            j = json{v.X(), v.Y(), v.Z()};
        }

        static void from_json(const json &j, gp_Vec &v)
        {
            v.SetCoord(j[0].get<double>(), j[1].get<double>(), j[2].get<double>());
        }
    };

    template <>
    struct adl_serializer<gp_Dir>
    {
        static void to_json(json &j, const gp_Dir &d)
        {
            j = json{d.X(), d.Y(), d.Z()};
        }

        static void from_json(const json &j, gp_Dir &d)
        {
            // gp_Dir constructor automatically normalizes the vector
            d = gp_Dir(j[0].get<double>(), j[1].get<double>(), j[2].get<double>());
        }
    };

    template <>
    struct adl_serializer<gp_Pnt2d>
    {
        static void to_json(json &j, const gp_Pnt2d &p)
        {
            j = json{p.X(), p.Y()};
        }

        static void from_json(const json &j, gp_Pnt2d &p)
        {
            p.SetCoord(j[0].get<double>(), j[1].get<double>());
        }
    };
}
