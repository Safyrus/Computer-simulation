#ifndef IODEVICE_HPP
#define IODEVICE_HPP

#include "computer/Device.hpp"
#include <memory>
#include "SFML/Window.hpp"

namespace computer
{
    class IODevice : public computer::Device
    {
    protected:
        uint8_t defaultPort;
        std::shared_ptr<IODevice> connectedDevice;

    public:
        IODevice();
        IODevice(uint8_t defaultPort);
        ~IODevice();

        virtual bool getTalk(uint8_t port) = 0;
        virtual void setTalk(uint8_t port, bool talk) = 0;
        virtual void send(uint8_t port, uint8_t data) = 0;

        virtual void inputEvent(sf::Event event);

        void setDefaultPort(uint8_t port);
        uint8_t getDefaultPort();
        void setConnectedDevice(std::shared_ptr<IODevice> connectedDevice);

    };
} // namespace computer

#endif // IODEVICE_HPP