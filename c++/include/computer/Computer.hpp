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
    public:
        Computer();
        Computer(bool test);
        ~Computer();

        void power();
        void reset();
        bool getPower();

        std::shared_ptr<computer::CPU> getCpu();
        std::shared_ptr<computer::Bus> getBus();

        void addDevice(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
        void removeDevice(std::string type, uint16_t startAdr, uint16_t endAdr);
        std::shared_ptr<computer::Device> getDevice(std::string type, uint16_t startAdr, uint16_t endAdr);
    };
} // namespace computer

#endif // COMPUTER_HPP