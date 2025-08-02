#pragma once

#include <string>
#include "nlohmann/json.hpp"
#include "transport/transport_interface.h"
#include "protocol/jsonrpc.h"

// using namespace JSONRPC;
typedef std::function<nlohmann::json(std::string method, const nlohmann::json &params)> RequestFunctionType;
typedef std::function<void(const nlohmann::json &)> ResponseFunctionType;
typedef std::function<void(std::string code, std::string message)> ErrorFunctionType;

class RPCHandler
{
public:
    RPCHandler(std::unique_ptr<ITransport> transporter) : _transporter(std::move(transporter)) {};
    // nlohmann::json handle_request(const std::string &request);
    void OnRequestHandler(RequestFunctionType &function);
    // void OnResponseHandler(std::function<void(const nlohmann::json &)> function);
    // void OnErrorHandler(std::function<void(const JSONRPC::JSONRPCError::ErrorType &)> function);

    void SendMessage(std::string method, nlohmann::json params, ResponseFunctionType onResponse = nullptr, ErrorFunctionType onError = nullptr);

    void Listen();

private:
    void handleMessage(const nlohmann::json &j);

    int _currentid = 0;
    std::unique_ptr<ITransport> _transporter;

    RequestFunctionType _onRequest = nullptr;
    std::map<int, ResponseFunctionType> _onResponseMap = {};
    std::map<int, ErrorFunctionType> _onErrorMap = {};
};