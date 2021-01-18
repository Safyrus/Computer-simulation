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

    addDevice(DEVICE_TYPE::ROM, 0x0000, 0x1000);
    addDevice(DEVICE_TYPE::RAM, 0x8000, 0x9000);
    std::shared_ptr<computer::ROM> rom = std::static_pointer_cast<computer::ROM>(getDevice(DEVICE_TYPE::ROM, 0x0000, 0x1000));
    rom->load("prog/verifCPU/verifCPU");

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

void computer::Computer::addDevice(DEVICE_TYPE type, uint16_t startAdr, uint16_t endAdr)
{
    switch (type)
    {
    case DEVICE_TYPE::RAM:
        addRAM(startAdr, endAdr);
        break;
    case DEVICE_TYPE::ROM:
        addROM(startAdr, endAdr);
        break;
    default:
        break;
    }
}

void computer::Computer::removeDevice(DEVICE_TYPE type, uint16_t startAdr, uint16_t endAdr)
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

std::shared_ptr<computer::Device> computer::Computer::getDevice(DEVICE_TYPE type, uint16_t startAdr, uint16_t endAdr)
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

void computer::Computer::addRAM(uint16_t startAdr, uint16_t endAdr)
{
    printDebug("Add RAM at " + std::to_string(startAdr) + "  " + std::to_string(endAdr));
    std::shared_ptr<computer::RAM> ram = std::make_shared<computer::RAM>(0x1000, 0);
    devices.push_back(ram);
    runnables.push_back(new computer::RunnableDevice(ram));
    runnables.back()->run();
    //ram->load("prog/verifCPU/verifCPU");
    ram->setPwr(cpu->getPwr());
    bus->addDevice(ram, startAdr, endAdr);
}

void computer::Computer::addROM(uint16_t startAdr, uint16_t endAdr)
{
    printDebug("Add ROM at " + std::to_string(startAdr) + "  " + std::to_string(endAdr));
    std::shared_ptr<computer::ROM> rom = std::make_shared<computer::ROM>(0x1000, 0);
    devices.push_back(rom);
    runnables.push_back(new computer::RunnableDevice(rom));
    runnables.back()->run();
    rom->setPwr(cpu->getPwr());
    bus->addDevice(rom, startAdr, endAdr);
}
