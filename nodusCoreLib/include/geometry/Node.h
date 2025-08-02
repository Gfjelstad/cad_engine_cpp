#ifndef Node_H
#define Node_H

#include "nlohmann/json.hpp"

namespace nodus::core::geometry
{

    template <typename T>
    std::pair<bool, T> try_convert_json(const nlohmann::json &j)
    {
        try
        {
            // Add some debug info
            static_assert(std::is_default_constructible_v<T>,
                          "Type T must be default constructible");

            return {true, j.template get<T>()};
        }
        catch (const nlohmann::json::exception &e)
        {
            // More specific exception handling for debugging
            return {false, T{}};
        }
        catch (...)
        {
            return {false, T{}};
        }
    }

    // template<typename T>
    // constexpr bool can_convert_from_json_v = can_convert_from_json<T>::value;

    template <typename NodeResult>
    class Node
    {
    private:
        /* data */
    public:
        Node() = default;

        virtual NodeResult Build() = 0;
        ~Node() {};
    };
};
#endif