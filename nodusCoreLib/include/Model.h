#ifndef Model_H
#define Model_H
#include "geometry/Node.h"
#include <iostream>
#include "nlohmann/json.hpp"
#include <memory>
#include <map>
#include <string>
using namespace nlohmann;
namespace nodus::core
{
    using namespace geometry;
    class Model
    {
    public:
        Model(nlohmann::json modeljson);
        ~Model();

        TopoDS_Shape Render(std::string id = "");

    private:
        std::shared_ptr<Node> PopulateModel(nlohmann::json modeljson, std::shared_ptr<Node> parent = nullptr);

        std::map<std::string, std::shared_ptr<Node>> nodes_;
        std::shared_ptr<Node> root_;
    };
}

#endif