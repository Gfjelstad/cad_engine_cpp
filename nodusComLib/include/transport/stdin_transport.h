#ifndef StdInTransport_H
#define StdInTransport_H

#include "transport_interface.h"
#include <iostream>

/// @brief standard in message transporting
class StdInTransport : public ITransport
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
#endif