#ifndef BUS_HPP
#define BUS_HPP

#include <vector>
#include <memory>

#include "computer/Device.hpp"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

namespace computer
{
    class Bus : public computer::Device
    {
    private:
        std::vector<std::shared_ptr<computer::Device>> devices;
        std::vector<uint16_t> startAdrs;
        std::vector<uint16_t> endAdrs;

    public:
        Bus();
        ~Bus();

        void reset();
        void setPwr(bool pwr);

        void run();
        uint8_t get(uint16_t adr);
        void set(uint16_t adr, uint8_t data);

        void addDevice(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
        void removeDevice(std::shared_ptr<computer::Device> device);
        uint32_t getDeviceAdr(std::shared_ptr<computer::Device> device);
    };
} // namespace computer

#endif // BUS_HPP