#ifndef HARDWARESTATES_HPP
#define HARDWARESTATES_HPP

#include <memory>

#include "computer/Device.hpp"

namespace computer
{
    class HardwareStates : public computer::Device
    {
    private:
        uint8_t connected;

    public:
        HardwareStates();
        ~HardwareStates();

        void reset();
        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);

        void connect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
        void disconnect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
    };
} // namespace computer

#endif // HARDWARESTATES_HPP