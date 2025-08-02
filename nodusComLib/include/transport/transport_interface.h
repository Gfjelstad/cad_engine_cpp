#ifndef ITransport_H
#define ITransport_H

#include <iostream>
class ITransport
{
public:
    virtual ~ITransport() = default;
    virtual std::string recieveMessage() = 0;
    virtual void sendMessage(std::string message) = 0;
    virtual bool isConnected() = 0;

    bool _connected = false;
};

#endif
