#include "computer/IODevice.hpp"

computer::IODevice::IODevice()
{
    type = "IO";
    this->defaultPort = 0;
}

computer::IODevice::IODevice(uint8_t defaultPort)
{
    type = "IO";
    this->defaultPort = defaultPort;
}

computer::IODevice::~IODevice()
{
}

void computer::IODevice::setDefaultPort(uint8_t port)
{
    this->defaultPort = port;
}

uint8_t computer::IODevice::getDefaultPort()
{
    return defaultPort;
}

void computer::IODevice::setConnectedDevice(std::shared_ptr<IODevice> connectedDevice)
{
    this->connectedDevice = connectedDevice;
}

void computer::IODevice::inputEvent(sf::Event event)
{
    switch (event.type)
    {
    default:
        break;
    }
}
