#include "transport_interface.h"
#include <iostream>

/// @brief standard in message transporting
class StdInTransport : ITransport
{
public:
    StdInTransport()
    {
        _connected = true;
    }

    virtual std::string recieveMessage() override;

    virtual void sendMessage(std::string message) override;

    virtual bool isConnected() override;
};