#ifndef NodusHandler_H
#define NodusHandler_H

#include "handler.h"
#include "transport/stdin_transport.h"

class NodusHandler : public RPCHandler
{
    NodusHandler() : RPCHandler(std::make_unique<StdInTransport>()) {};

protected:
    virtual nlohmann::json RequestHandler(std::string method, const nlohmann::json &params) override
    {
        if (method == "render")
        {
        }
    }
};

#endif