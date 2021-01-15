#include "computer/Computer.hpp"
#include "computer/RAM.hpp"
#include "computer/RunnableDevice.hpp"

#include <iostream>

computer::Computer::Computer()
{
    std::cout << "\nCreate BUS" << std::endl;
    bus = std::make_shared<computer::Bus>();

    std::cout << "Create CPU thread" << std::endl;
    cpu = std::make_shared<computer::CPU>(bus, true);
    cpu->hz = 10;
    runCPU = new RunnableDevice(cpu);

    addDevice(DEVICE_TYPE::RAM, 0x0000, 0x1000);

    std::cout << "Run CPU thread" << std::endl;
    runCPU->run();

    cpu->setPwr(false);
}

computer::Computer::~Computer()
{
    runCPU->stop();
    std::cout << "Wait for CPU thread" << std::endl;
    runCPU->join();
    std::cout << "CPU thread stop" << std::endl;
    delete runCPU;

    std::cout << "Wait for devices threads" << std::endl;
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        runnables[i]->stop();
        runnables[i]->join();
        delete runnables[i];
    }
    std::cout << "Devices threads stop" << std::endl;
}

void computer::Computer::power()
{
    bool pwr = !cpu->getPwr();
    std::cout << "pwr=" << pwr << "\n";
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
        std::cout << "Add RAM" << std::endl;
        addRAM(startAdr, endAdr);
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
    std::cout << "Add RAM at " << startAdr << "  " << endAdr << "\n";
    std::shared_ptr<computer::RAM> ram = std::make_shared<computer::RAM>(0x1000, 0);
    devices.push_back(ram);
    runnables.push_back(new computer::RunnableDevice(ram));
    runnables.back()->run();
    ram->load("prog/verifCPU/verifCPU");
    ram->setPwr(cpu->getPwr());
    bus->addDevice(ram, startAdr, endAdr);
}
