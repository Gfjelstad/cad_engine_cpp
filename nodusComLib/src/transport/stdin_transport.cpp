#include "stdin_transport.h"

std::string StdInTransport::recieveMessage()
{
    std::string line;
    if (std::getline(std::cin, line))
    {
        return line;
    }
    _connected = false;
    throw std::exception();
}
void StdInTransport::sendMessage(std::string message)
{
    std::cout << message << std::endl;
}

bool StdInTransport::isConnected()
{
    return std::cin.good() && _connected;
}
