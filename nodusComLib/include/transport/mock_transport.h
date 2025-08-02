#ifndef MockTransport_H
#define MockTransport_H

#include "transport_interface.h"
#include <iostream>
#include <vector>

/// @brief standard in message transporting
class MockTransport : public ITransport
{
public:
    MockTransport(std::vector<std::string> messages, std::shared_ptr<std::vector<std::string>> output = nullptr) : ITransport()
    {
        _messages = messages;
        _output = output;
        _connected = true;
    }

    virtual std::string recieveMessage() override
    {
        if (current_ >= _messages.size())
        {
            _connected = false;
            throw std::runtime_error("Disconnected");
        }
        return _messages[current_++];
    };

    virtual void sendMessage(std::string message) override
    {
        if (_output != nullptr)
        {
            _output->push_back(message);
        }
    };

    virtual bool isConnected() override
    {
        return _connected;
    };
    size_t current_ = 0;
    std::vector<std::string> _messages;
    std::shared_ptr<std::vector<std::string>> _output;
};

#endif