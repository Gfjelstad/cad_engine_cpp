#ifndef NodusHandler_H
#define NodusHandler_H

#include "handler.h"
#include "transport/stdin_transport.h"
#include "Model.h"
#include <iostream>
#include <map>

using namespace nodus::core;

class NodusHandler : public RPCHandler
{
    NodusHandler() : RPCHandler(std::make_unique<StdInTransport>()) {};

protected:
    virtual nlohmann::json RequestHandler(std::string method, const nlohmann::json &params) override
    {
        if (method == "model/create")
        {
            std::string id = params.at("id").get<std::string>();
            nlohmann::json model = params.at("model");

            Model model(model);
        }
        if (method == "model/update")
        {
        }
    }

    std::map<std::string, Model>
};

#endif