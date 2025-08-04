#pragma once
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
            if (!j.is_array())
            {
                throw json::type_error::create(302, "type must be array, but is " + std::string(j.type_name()), &j);
            }
            if (j.size() != 3)
            {
                throw json::out_of_range::create(401, "array must be length 3", &j);
            }
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
            if (!j.is_array())
            {
                throw json::type_error::create(302, "type must be array, but is " + std::string(j.type_name()), &j);
            }
            if (j.size() != 3)
            {
                throw json::out_of_range::create(401, "array must be length 3", &j);
            }
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
            if (!j.is_array())
            {
                throw json::type_error::create(302, "type must be array, but is " + std::string(j.type_name()), &j);
            }
            if (j.size() != 3)
            {
                throw json::out_of_range::create(401, "array must be length 3", &j);
            }
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
            if (!j.is_array())
            {
                throw json::type_error::create(302, "type must be array, but is " + std::string(j.type_name()), &j);
            }
            if (j.size() != 2)
            {
                throw json::out_of_range::create(401, "array must be length 2", &j);
            }
            p.SetCoord(j[0].get<double>(), j[1].get<double>());
        }
    };
    template <>
    struct adl_serializer<gp_Ax3>
    {
        static void to_json(json &j, const gp_Ax3 &p)
        {
            j["location"] = p.Location();
            j["direction"] = p.Direction();
        }

        static void from_json(const json &j, gp_Ax3 &p)
        {
            if (!j.is_object())
            {
                throw json::type_error::create(302, "type must be object, but is " + std::string(j.type_name()), &j);
            }

            if (!j.contains("location"))
            {
                throw json::out_of_range::create(403, "key 'location' not found", &j);
            }
            if (!j.contains("direction"))
            {
                throw json::out_of_range::create(403, "key 'direction' not found", &j);
            }
            p = gp_Ax3(j["location"].get<gp_Pnt>(), j["direction"].get<gp_Vec>());
        }
    };
}
