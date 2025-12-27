#ifndef Node_H
#define Node_H

#include "nlohmann/json.hpp"
#include <gp_Ax3.hxx>
#include <TopoDS_Shape.hxx>

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

    class Node
    {
    protected:
        /* data */
        std::string _id;
        std::shared_ptr<Node> _parent;
        std::vector<std::shared_ptr<Node>> _children;
        nlohmann::json _params;

    public:
        Node(nlohmann::json params, std::shared_ptr<Node> parent)
        {
            params.at("id").get_to(_id);
            _params = params;
            _parent = parent;
        };
        virtual ~Node() = default;

        void SetChildren(std::vector<std::shared_ptr<Node>> children)
        {
            _children = children;
        }

        std::string GetId()
        {
            return _id;
        }

        virtual gp_Ax3 GetOrigin()
        {
            if (_parent == nullptr)
            {
                return gp_Ax3();
            }
            return _parent->GetOrigin();
        }

        virtual TopoDS_Shape Build() = 0;

        virtual std::string Render();
    };

    std::shared_ptr<Node> CreateNode(const nlohmann::json &node, std::shared_ptr<Node> parent = nullptr);
}
#endif