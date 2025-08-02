#ifndef RPCHandler_H
#define RPCHandler_H

#include <string>
#include "nlohmann/json.hpp"
#include "transport/transport_interface.h"
#include "protocol/jsonrpc.h"

// using namespace JSONRPC;
typedef std::function<void(const nlohmann::json &)> ResponseFunctionType;
typedef std::function<void(std::string code, std::string message)> ErrorFunctionType;

class RPCHandler
{
public:
    RPCHandler(std::unique_ptr<ITransport> transporter) : _transporter(std::move(transporter)) {};

    void SendMessage(std::string method, nlohmann::json params, ResponseFunctionType onResponse = nullptr, ErrorFunctionType onError = nullptr);
    void Listen();

protected:
    virtual nlohmann::json RequestHandler(std::string method, const nlohmann::json &params) = 0;

private:
    void handleMessage(const nlohmann::json &j);

    int _currentid = 0;
    std::unique_ptr<ITransport> _transporter;

    std::map<int, ResponseFunctionType> _onResponseMap = {};
    std::map<int, ErrorFunctionType> _onErrorMap = {};
};

#endif