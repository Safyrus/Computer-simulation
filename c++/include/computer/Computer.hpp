#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <memory>

#include "computer/Device.hpp"
#include "computer/Bus.hpp"
#include "computer/CPU.hpp"
#include "computer/RunnableDevice.hpp"

namespace computer
{
    class Computer
    {
    protected:
        std::shared_ptr<computer::CPU> cpu;
        std::shared_ptr<computer::Bus> bus;
        std::vector<std::shared_ptr<computer::Device>> devices;

        std::vector<computer::RunnableDevice*> runnables;
        computer::RunnableDevice* runCPU;

        void addRAM(uint16_t startAdr, uint16_t endAdr);
        void addROM(uint16_t startAdr, uint16_t endAdr);

    public:
        Computer();
        ~Computer();

        void power();
        void reset();
        bool getPower();

        std::shared_ptr<computer::CPU> getCpu();
        std::shared_ptr<computer::Bus> getBus();

        void addDevice(DEVICE_TYPE type, uint16_t startAdr, uint16_t endAdr);
        void removeDevice(DEVICE_TYPE type, uint16_t startAdr, uint16_t endAdr);
        std::shared_ptr<computer::Device> getDevice(DEVICE_TYPE type, uint16_t startAdr, uint16_t endAdr);
    };
} // namespace computer

#endif // COMPUTER_HPP