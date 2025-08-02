#include "handler.h"
#include <iostream>
#include "protocol/jsonrpc.h"

using namespace JSONRPC;

void RPCHandler::SendMessage(std::string method, nlohmann::json params, ResponseFunctionType onResponse, ErrorFunctionType onError)
{
    JSONRPCRequest request;
    request.id = _currentid++;
    request.method = method;
    request.params = params;

    if (onResponse != nullptr)
    {
        _onResponseMap.insert({request.id, onResponse});
    }
    if (onError != nullptr)
    {
        _onErrorMap.insert({request.id, onError});
    }
}

void RPCHandler::Listen()
{
    while (_transporter->isConnected())
    {
        std::string message;
        try
        {
            message = _transporter->recieveMessage();
        }
        catch (const std::exception &e)
        {
            break;
            std::cerr << "disconnected" << '\n';
        }

        auto j = nlohmann::json::parse(message);

        handleMessage(j);
    }
}

void RPCHandler::handleMessage(const nlohmann::json &j)
{
    if (j.contains("method"))
    {
        // Likely a JSON-RPC Request
        JSONRPCRequest req = j.get<JSONRPCRequest>();
        int id = req.id;
        nlohmann::json result = RequestHandler(req.method, req.params);

        JSONRPCResponse response;
        response.jsonrpc = "2.0";
        response.id = id;
        response.result = result;
        nlohmann::json r = response;
        std::string strresponse = r.dump();
        _transporter->sendMessage(strresponse);
    }
    else if (j.contains("result"))
    {
        // Likely a Response
        JSONRPCResponse res = j.get<JSONRPCResponse>();

        if (_onResponseMap.find(res.id) != _onResponseMap.end())
        {
            _onResponseMap[res.id](res.result);
            _onResponseMap.erase(res.id);
        }
        _onErrorMap.erase(res.id);
    }
    else if (j.contains("error"))
    {
        // Likely an Error
        JSONRPCError err = j.get<JSONRPCError>();
        if (_onErrorMap.find(err.id) != _onErrorMap.end())
        {
            _onErrorMap[err.id](err.error.code, err.error.message);
            _onErrorMap.erase(err.id);
        }
        _onResponseMap.erase(err.id);
    }
    else
    {
        std::cerr << "Unknown JSON-RPC message format: " << j.dump() << std::endl;
    }
}
