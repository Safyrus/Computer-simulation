#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <memory>

#include "computer/Device.hpp"
#include "computer/Bus.hpp"
#include "computer/CPU.hpp"
#include "computer/HardwareStates.hpp"
#include "computer/RunnableDevice.hpp"
#include "computer/IODevice.hpp"
#include "data/Floppy.hpp"

namespace computer
{
    class Computer
    {
    protected:
        std::shared_ptr<computer::CPU> cpu;
        std::shared_ptr<computer::Bus> bus;
        std::shared_ptr<computer::HardwareStates> hwStats;
        std::vector<std::shared_ptr<computer::Device>> devices;

        std::vector<computer::RunnableDevice *> runnables;
        computer::RunnableDevice *runCPU;

    public:
        Computer();
        Computer(bool test, std::string prog, uint32_t hz = 0, bool printCPU = false);
        ~Computer();

        void power();
        void reset();
        bool getPower();
        bool isDriveActive();
        bool isDriveLock();
        bool isFloppyIn();

        std::shared_ptr<computer::CPU> getCpu();
        std::shared_ptr<computer::Bus> getBus();

        void addDevice(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
        void removeDevice(std::string type, uint16_t startAdr, uint16_t endAdr);
        void removeAllDevices();
        std::shared_ptr<computer::Device> getDevice(std::string type, uint16_t startAdr, uint16_t endAdr);
        std::vector<std::shared_ptr<computer::Device>> getAllDevice();

        void connectIODevice(std::shared_ptr<computer::IODevice> device, uint8_t port);
        void removeIODevice(uint8_t port);
    };
} // namespace computer

#endif // COMPUTER_HPP