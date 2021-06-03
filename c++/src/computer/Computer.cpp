#include "computer/Computer.hpp"
#include "computer/RAM.hpp"
#include "computer/ROM.hpp"
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
        std::shared_ptr<computer::ROM> rom = std::make_shared<computer::ROM>(0x1000);
        std::shared_ptr<computer::RAM> ram = std::make_shared<computer::RAM>(0x0400);
        addDevice(rom, 0x0000, 0x0FFF);
        addDevice(ram, 0x1800, 0x1BFF);
        rom = std::static_pointer_cast<computer::ROM>(getDevice("ROM", 0x0000, 0x0FFF));
        rom->load(prog);
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
