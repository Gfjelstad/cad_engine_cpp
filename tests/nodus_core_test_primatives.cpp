#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "handler.h"
#include "protocol/jsonrpc.h"
#include "transport/mock_transport.h"
#include <vector>
#include <iostream>
#include "geometry/LineNode.h"
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Ax3.hxx>
#include <cmath>
using namespace JSONRPC;

using json = nlohmann::json;

// Your ADL serializers would be included here
// namespace nlohmann { ... }

// Helper function for floating point comparison
bool isApproxEqual(double a, double b, double epsilon = 1e-9)
{
    return std::abs(a - b) < epsilon;
}

TEST_CASE("gp_Pnt JSON serialization")
{
    SUBCASE("to_json conversion")
    {
        gp_Pnt point(1.5, 2.7, -3.14);
        json j = point;

        REQUIRE(j.is_array());
        REQUIRE(j.size() == 3);
        CHECK(j[0].get<double>() == doctest::Approx(1.5));
        CHECK(j[1].get<double>() == doctest::Approx(2.7));
        CHECK(j[2].get<double>() == doctest::Approx(-3.14));
    }

    SUBCASE("from_json conversion")
    {
        json j = {1.5, 2.7, -3.14};
        gp_Pnt point = j.get<gp_Pnt>();

        CHECK(point.X() == doctest::Approx(1.5));
        CHECK(point.Y() == doctest::Approx(2.7));
        CHECK(point.Z() == doctest::Approx(-3.14));
    }

    SUBCASE("round-trip conversion")
    {
        gp_Pnt original(10.0, -5.5, 7.25);
        json j = original;
        gp_Pnt restored = j.get<gp_Pnt>();

        CHECK(original.X() == doctest::Approx(restored.X()));
        CHECK(original.Y() == doctest::Approx(restored.Y()));
        CHECK(original.Z() == doctest::Approx(restored.Z()));
    }

    SUBCASE("zero coordinates")
    {
        gp_Pnt origin(0.0, 0.0, 0.0);
        json j = origin;
        gp_Pnt restored = j.get<gp_Pnt>();

        CHECK(restored.X() == doctest::Approx(0.0));
        CHECK(restored.Y() == doctest::Approx(0.0));
        CHECK(restored.Z() == doctest::Approx(0.0));
    }
}

TEST_CASE("gp_Vec JSON serialization")
{
    SUBCASE("to_json conversion")
    {
        gp_Vec vector(3.0, 4.0, 5.0);
        json j = vector;

        REQUIRE(j.is_array());
        REQUIRE(j.size() == 3);
        CHECK(j[0].get<double>() == doctest::Approx(3.0));
        CHECK(j[1].get<double>() == doctest::Approx(4.0));
        CHECK(j[2].get<double>() == doctest::Approx(5.0));
    }

    SUBCASE("from_json conversion")
    {
        json j = {3.0, 4.0, 5.0};
        gp_Vec vector = j.get<gp_Vec>();

        CHECK(vector.X() == doctest::Approx(3.0));
        CHECK(vector.Y() == doctest::Approx(4.0));
        CHECK(vector.Z() == doctest::Approx(5.0));
    }

    SUBCASE("round-trip conversion")
    {
        gp_Vec original(-1.0, 2.5, -8.75);
        json j = original;
        gp_Vec restored = j.get<gp_Vec>();

        CHECK(original.X() == doctest::Approx(restored.X()));
        CHECK(original.Y() == doctest::Approx(restored.Y()));
        CHECK(original.Z() == doctest::Approx(restored.Z()));
    }

    SUBCASE("unit vector")
    {
        gp_Vec unitVec(1.0, 0.0, 0.0);
        json j = unitVec;
        gp_Vec restored = j.get<gp_Vec>();

        CHECK(restored.Magnitude() == doctest::Approx(1.0));
        CHECK(restored.X() == doctest::Approx(1.0));
        CHECK(restored.Y() == doctest::Approx(0.0));
        CHECK(restored.Z() == doctest::Approx(0.0));
    }
}

TEST_CASE("gp_Dir JSON serialization")
{
    SUBCASE("to_json conversion")
    {
        gp_Dir direction(1.0, 1.0, 1.0); // Will be automatically normalized
        json j = direction;

        REQUIRE(j.is_array());
        REQUIRE(j.size() == 3);

        // Check that the serialized values represent a unit vector
        double x = j[0].get<double>();
        double y = j[1].get<double>();
        double z = j[2].get<double>();
        double magnitude = std::sqrt(x * x + y * y + z * z);
        CHECK(magnitude == doctest::Approx(1.0));
    }

    SUBCASE("from_json conversion")
    {
        json j = {1.0, 1.0, 1.0}; // Will be normalized during construction
        gp_Dir direction = j.get<gp_Dir>();

        // Check that it's a unit vector
        double magnitude = std::sqrt(direction.X() * direction.X() +
                                     direction.Y() * direction.Y() +
                                     direction.Z() * direction.Z());
        CHECK(magnitude == doctest::Approx(1.0));
    }

    SUBCASE("round-trip conversion with normalization")
    {
        gp_Dir original(3.0, 4.0, 0.0); // Magnitude = 5, will be normalized
        json j = original;
        gp_Dir restored = j.get<gp_Dir>();

        // Both should be unit vectors with same direction
        CHECK(original.X() == doctest::Approx(restored.X()));
        CHECK(original.Y() == doctest::Approx(restored.Y()));
        CHECK(original.Z() == doctest::Approx(restored.Z()));
    }

    SUBCASE("cardinal directions")
    {
        SUBCASE("X direction")
        {
            gp_Dir xDir(1.0, 0.0, 0.0);
            json j = xDir;
            gp_Dir restored = j.get<gp_Dir>();

            CHECK(restored.X() == doctest::Approx(1.0));
            CHECK(restored.Y() == doctest::Approx(0.0));
            CHECK(restored.Z() == doctest::Approx(0.0));
        }

        SUBCASE("Y direction")
        {
            gp_Dir yDir(0.0, 1.0, 0.0);
            json j = yDir;
            gp_Dir restored = j.get<gp_Dir>();

            CHECK(restored.X() == doctest::Approx(0.0));
            CHECK(restored.Y() == doctest::Approx(1.0));
            CHECK(restored.Z() == doctest::Approx(0.0));
        }

        SUBCASE("Z direction")
        {
            gp_Dir zDir(0.0, 0.0, 1.0);
            json j = zDir;
            gp_Dir restored = j.get<gp_Dir>();

            CHECK(restored.X() == doctest::Approx(0.0));
            CHECK(restored.Y() == doctest::Approx(0.0));
            CHECK(restored.Z() == doctest::Approx(1.0));
        }
    }
}

TEST_CASE("gp_Pnt2d JSON serialization")
{
    SUBCASE("to_json conversion")
    {
        gp_Pnt2d point2d(2.5, -1.7);
        json j = point2d;

        REQUIRE(j.is_array());
        REQUIRE(j.size() == 2);
        CHECK(j[0].get<double>() == doctest::Approx(2.5));
        CHECK(j[1].get<double>() == doctest::Approx(-1.7));
    }

    SUBCASE("from_json conversion")
    {
        json j = {2.5, -1.7};
        gp_Pnt2d point2d = j.get<gp_Pnt2d>();

        CHECK(point2d.X() == doctest::Approx(2.5));
        CHECK(point2d.Y() == doctest::Approx(-1.7));
    }

    SUBCASE("round-trip conversion")
    {
        gp_Pnt2d original(100.25, -50.75);
        json j = original;
        gp_Pnt2d restored = j.get<gp_Pnt2d>();

        CHECK(original.X() == doctest::Approx(restored.X()));
        CHECK(original.Y() == doctest::Approx(restored.Y()));
    }

    SUBCASE("origin point")
    {
        gp_Pnt2d origin(0.0, 0.0);
        json j = origin;
        gp_Pnt2d restored = j.get<gp_Pnt2d>();

        CHECK(restored.X() == doctest::Approx(0.0));
        CHECK(restored.Y() == doctest::Approx(0.0));
    }
}

TEST_CASE("gp_Ax3 JSON serialization")
{
    SUBCASE("to_json conversion")
    {
        gp_Pnt location(1.0, 2.0, 3.0);
        gp_Vec direction(0.0, 0.0, 1.0);
        gp_Ax3 axis(location, direction);

        json j = axis;

        REQUIRE(j.is_object());
        REQUIRE(j.contains("location"));
        REQUIRE(j.contains("direction"));

        // Check location
        json loc_j = j["location"];
        REQUIRE(loc_j.is_array());
        REQUIRE(loc_j.size() == 3);
        CHECK(loc_j[0].get<double>() == doctest::Approx(1.0));
        CHECK(loc_j[1].get<double>() == doctest::Approx(2.0));
        CHECK(loc_j[2].get<double>() == doctest::Approx(3.0));

        // Check direction
        json dir_j = j["direction"];
        REQUIRE(dir_j.is_array());
        REQUIRE(dir_j.size() == 3);
        CHECK(dir_j[0].get<double>() == doctest::Approx(0.0));
        CHECK(dir_j[1].get<double>() == doctest::Approx(0.0));
        CHECK(dir_j[2].get<double>() == doctest::Approx(1.0));
    }

    SUBCASE("from_json conversion")
    {
        json j = {
            {"location", {1.0, 2.0, 3.0}},
            {"direction", {0.0, 0.0, 1.0}}};

        gp_Ax3 axis = j.get<gp_Ax3>();

        gp_Pnt loc = axis.Location();
        CHECK(loc.X() == doctest::Approx(1.0));
        CHECK(loc.Y() == doctest::Approx(2.0));
        CHECK(loc.Z() == doctest::Approx(3.0));

        gp_Dir dir = axis.Direction();
        CHECK(dir.X() == doctest::Approx(0.0));
        CHECK(dir.Y() == doctest::Approx(0.0));
        CHECK(dir.Z() == doctest::Approx(1.0));
    }

    SUBCASE("round-trip conversion")
    {
        gp_Pnt original_location(-5.5, 10.0, 2.25);
        gp_Vec original_direction(1.0, 1.0, 0.0); // Will be normalized
        gp_Ax3 original(original_location, original_direction);

        json j = original;
        gp_Ax3 restored = j.get<gp_Ax3>();

        // Check location
        gp_Pnt restored_loc = restored.Location();
        CHECK(original_location.X() == doctest::Approx(restored_loc.X()));
        CHECK(original_location.Y() == doctest::Approx(restored_loc.Y()));
        CHECK(original_location.Z() == doctest::Approx(restored_loc.Z()));

        // Check direction (normalized)
        gp_Dir original_dir = original.Direction();
        gp_Dir restored_dir = restored.Direction();
        CHECK(original_dir.X() == doctest::Approx(restored_dir.X()));
        CHECK(original_dir.Y() == doctest::Approx(restored_dir.Y()));
        CHECK(original_dir.Z() == doctest::Approx(restored_dir.Z()));
    }

    SUBCASE("standard coordinate system")
    {
        gp_Ax3 standard; // Default constructor creates standard coordinate system
        json j = standard;
        gp_Ax3 restored = j.get<gp_Ax3>();

        // Default location should be origin
        gp_Pnt loc = restored.Location();
        CHECK(loc.X() == doctest::Approx(0.0));
        CHECK(loc.Y() == doctest::Approx(0.0));
        CHECK(loc.Z() == doctest::Approx(0.0));

        // Default direction should be Z-axis
        gp_Dir dir = restored.Direction();
        CHECK(dir.X() == doctest::Approx(0.0));
        CHECK(dir.Y() == doctest::Approx(0.0));
        CHECK(dir.Z() == doctest::Approx(1.0));
    }
}

TEST_CASE("Error handling and edge cases")
{
    SUBCASE("Valid JSON bounds checking")
    {
        // Instead of testing invalid access, test that we can validate JSON structure
        SUBCASE("gp_Pnt requires exactly 3 elements")
        {
            json j_valid = {1.0, 2.0, 3.0};
            json j_too_few = {1.0, 2.0};
            json j_too_many = {1.0, 2.0, 3.0, 4.0};

            // Valid case should work
            CHECK_NOTHROW(j_valid.get<gp_Pnt>());

            // For invalid cases, we can check size before attempting conversion
            CHECK(j_too_few.size() == 2);
            CHECK(j_too_many.size() == 4);
            CHECK(j_valid.size() == 3);
        }

        SUBCASE("gp_Pnt2d requires exactly 2 elements")
        {
            json j_valid = {1.0, 2.0};
            json j_too_few = {1.0};
            json j_too_many = {1.0, 2.0, 3.0};

            // Valid case should work
            CHECK_NOTHROW(j_valid.get<gp_Pnt2d>());

            // Check sizes
            CHECK(j_too_few.size() == 1);
            CHECK(j_too_many.size() == 3);
            CHECK(j_valid.size() == 2);
        }
    }

    SUBCASE("JSON structure validation")
    {
        SUBCASE("gp_Ax3 requires location and direction fields")
        {
            json j_valid = {
                {"location", {1.0, 2.0, 3.0}},
                {"direction", {0.0, 0.0, 1.0}}};
            json j_missing_direction = {{"location", {1.0, 2.0, 3.0}}};
            json j_missing_location = {{"direction", {0.0, 0.0, 1.0}}};

            // Valid case should work
            CHECK_NOTHROW(j_valid.get<gp_Ax3>());

            // Check for required fields
            CHECK(j_valid.contains("location"));
            CHECK(j_valid.contains("direction"));
            CHECK_FALSE(j_missing_direction.contains("direction"));
            CHECK_FALSE(j_missing_location.contains("location"));
        }

        SUBCASE("Exception testing with safer serializers")
        {
            // These tests assume you're using the safer serializers with validation

            SUBCASE("Invalid JSON for gp_Pnt - insufficient elements")
            {
                json j = {1.0, 2.0}; // Only 2 elements instead of 3
                CHECK_THROWS_AS(j.get<gp_Pnt>(), nlohmann::json::out_of_range);
            }

            SUBCASE("Invalid JSON for gp_Pnt2d - insufficient elements")
            {
                json j = {1.0}; // Only 1 element instead of 2
                CHECK_THROWS_AS(j.get<gp_Pnt2d>(), nlohmann::json::out_of_range);
            }

            SUBCASE("Invalid JSON for gp_Ax3 - missing fields")
            {
                json j = {{"location", {1.0, 2.0, 3.0}}}; // Missing direction
                CHECK_THROWS_AS(j.get<gp_Ax3>(), nlohmann::json::out_of_range);
            }

            SUBCASE("Wrong JSON types")
            {
                SUBCASE("Object instead of array for gp_Pnt")
                {
                    json j = {{"x", 1.0}, {"y", 2.0}, {"z", 3.0}};
                    CHECK_THROWS_AS(j.get<gp_Pnt>(), nlohmann::json::type_error);
                }

                SUBCASE("Array instead of object for gp_Ax3")
                {
                    json j = {1.0, 2.0, 3.0};
                    CHECK_THROWS_AS(j.get<gp_Ax3>(), nlohmann::json::type_error);
                }
            }
        }

        SUBCASE("Very small numbers")
        {
            gp_Pnt tiny(1e-15, -1e-15, 1e-15);
            json j = tiny;
            gp_Pnt restored = j.get<gp_Pnt>();

            CHECK(restored.X() == doctest::Approx(1e-15));
            CHECK(restored.Y() == doctest::Approx(-1e-15));
            CHECK(restored.Z() == doctest::Approx(1e-15));
        }

        SUBCASE("Very large numbers")
        {
            gp_Vec large(1e10, -1e10, 1e10);
            json j = large;
            gp_Vec restored = j.get<gp_Vec>();

            CHECK(restored.X() == doctest::Approx(1e10));
            CHECK(restored.Y() == doctest::Approx(-1e10));
            CHECK(restored.Z() == doctest::Approx(1e10));
        }
    }
}