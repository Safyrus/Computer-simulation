#include "computer/Computer.hpp"
#include "computer/RAM.hpp"
#include "computer/ROM.hpp"
#include "computer/VPU.hpp"
#include "computer/RunnableDevice.hpp"

#include "utils/console.hpp"

#include <iostream>

computer::Computer::Computer()
{
    printDebug("Create BUS");
    bus = std::make_shared<computer::Bus>();

    printDebug("Create CPU thread");
    cpu = std::make_shared<computer::CPU>(bus, true);
    cpu->hz = 10;
    runCPU = new RunnableDevice(cpu);

    printDebug("Run CPU thread");
    runCPU->run();

    cpu->setPwr(false);
}

computer::Computer::Computer(bool test, std::string prog)
{
    printDebug("Create BUS");
    bus = std::make_shared<computer::Bus>();

    printDebug("Create CPU thread");
    cpu = std::make_shared<computer::CPU>(bus, true);
    cpu->hz = 10;
    runCPU = new RunnableDevice(cpu);

    if(test)
    {
        std::shared_ptr<computer::ROM> rom4k = std::make_shared<computer::ROM>(0x1000);
        std::shared_ptr<computer::ROM> rom2k = std::make_shared<computer::ROM>(0x0800);
        std::shared_ptr<computer::RAM> ram1k = std::make_shared<computer::RAM>(0x0400);
        std::shared_ptr<computer::RAM> ram8k = std::make_shared<computer::RAM>(0x2000);
        std::shared_ptr<computer::RAM> ram16k = std::make_shared<computer::RAM>(0x4000);
        std::shared_ptr<computer::VPU> vpu = std::make_shared<computer::VPU>(ram8k);
        addDevice(rom4k, 0x0000, 0x0FFF);
        addDevice(rom2k, 0x1000, 0x17FF);
        addDevice(ram1k, 0x1800, 0x1BFF);
        addDevice(ram8k, 0x2000, 0x3FFF);
        addDevice(ram16k, 0x4000, 0x7FFF);
        addDevice(vpu, 0x1C18, 0x1C1F);
        rom4k = std::static_pointer_cast<computer::ROM>(getDevice("ROM", 0x0000, 0x0FFF));
        rom4k->load(prog);
    }

    printDebug("Run CPU thread");
    runCPU->run();

    cpu->setPwr(false);
}

computer::Computer::~Computer()
{
    runCPU->stop();
    printDebug("Wait for CPU thread");
    runCPU->join();
    printDebug("CPU thread stop");
    delete runCPU;

    printDebug("Wait for devices thread");
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        runnables[i]->stop();
        runnables[i]->join();
        delete runnables[i];
    }
    printDebug("Devices thread stop");
}

void computer::Computer::power()
{
    bool pwr = !cpu->getPwr();
    cpu->setPwr(pwr);
}

void computer::Computer::reset()
{
    cpu->reset();
}

bool computer::Computer::getPower()
{
    return cpu->getPwr();
}

std::shared_ptr<computer::CPU> computer::Computer::getCpu()
{
    return cpu;
}

std::shared_ptr<computer::Bus> computer::Computer::getBus()
{
    return bus;
}

void computer::Computer::addDevice(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr)
{
    printDebug("Add " + device->getType() + " at " + std::to_string(startAdr) + "  " + std::to_string(endAdr));
    devices.push_back(device);
    runnables.push_back(new computer::RunnableDevice(device));
    runnables.back()->run();
    device->setPwr(cpu->getPwr());
    bus->addDevice(device, startAdr, endAdr);
}

void computer::Computer::removeDevice(std::string type, uint16_t startAdr, uint16_t endAdr)
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        uint32_t adr = bus->getDeviceAdr(devices[i]);
        uint16_t start = adr >> 16;
        uint16_t end = adr & 0xffff;
        if (devices[i]->getType() == type && start >= startAdr && end <= endAdr)
        {
            runnables[i]->stop();
            runnables[i]->join();
            delete runnables[i];
            runnables.erase(runnables.begin() + i);
            devices.erase(devices.begin() + i);
        }
    }
}

std::shared_ptr<computer::Device> computer::Computer::getDevice(std::string type, uint16_t startAdr, uint16_t endAdr)
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        uint32_t adr = bus->getDeviceAdr(devices[i]);
        uint16_t start = adr >> 16;
        uint16_t end = adr & 0xffff;
        if (devices[i]->getType() == type && start >= startAdr && end <= endAdr)
        {
            return devices[i];
        }
    }
    return nullptr;
}
