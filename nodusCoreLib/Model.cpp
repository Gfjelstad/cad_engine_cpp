#include "Model.h"

namespace nodus::core
{

    Model::Model(json modeljson)
    {
        root_ = PopulateModel(modeljson);
    }

    Model::~Model()
    {
    }

    TopoDS_Shape Model::Render(std::string id)
    {
        if (!id.empty() && nodes_[id] != nullptr)
        {
            return nodes_[id]->Build();
        }
        return root_->Build();
    }

    std::shared_ptr<Node> Model::PopulateModel(nlohmann::json modeljson, std::shared_ptr<Node> parent)
    {
        std::vector<json> children = {modeljson};

        auto node = geometry::CreateNode(modeljson, parent);

        nodes_[node->GetId()] = node;

        if (modeljson.contains("children") && modeljson.at("children").is_array())
        {
            auto children = modeljson.at("children").get<std::vector<json>>();
            std::vector<std::shared_ptr<Node>> childNodes;
            for (auto child : children)
            {
                childNodes.push_back(PopulateModel(child, node));
            }
            node->SetChildren(childNodes);
        }
        return node;
    }
}
