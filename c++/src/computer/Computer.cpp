#include "computer/Computer.hpp"
#include "computer/RAM.hpp"
#include "computer/ROM.hpp"
#include "computer/VPU.hpp"
#include "computer/IOController.hpp"
#include "computer/RunnableDevice.hpp"

#include "utils/console.hpp"

#include <iostream>

computer::Computer::Computer()
{
    printDebug("Create BUS");
    bus = std::make_shared<computer::Bus>();

    printDebug("Create CPU thread");
    cpu = std::make_shared<computer::CPU>(bus, true);
    cpu->hz = 15625;
    runCPU = new RunnableDevice(cpu);

    hwStats = std::make_shared<computer::HardwareStates>();
    addDevice(hwStats, 0x1C00, 0x1C20);

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
    cpu->hz = 15625;
    runCPU = new RunnableDevice(cpu);

    if (test)
    {
        printDebug("Create ROM4K");
        std::shared_ptr<computer::ROM> rom4k = std::make_shared<computer::ROM>(0x1000);
        rom4k->setName("ROM4K");
        printDebug("Create ROM2K");
        std::shared_ptr<computer::ROM> rom2k = std::make_shared<computer::ROM>(0x0800);
        rom2k->setName("ROM2K");
        printDebug("Create RAM1K");
        std::shared_ptr<computer::RAM> ram1k = std::make_shared<computer::RAM>(0x0400);
        ram1k->setName("RAM1K");
        printDebug("Create VRAM8K");
        std::shared_ptr<computer::VRAM> vram = std::make_shared<computer::VRAM>(0x2000);
        vram->setName("VRAM8K");
        printDebug("Create RAM16K");
        std::shared_ptr<computer::RAM> ram16k = std::make_shared<computer::RAM>(0x4000);
        ram16k->setName("RAM16K");
        printDebug("Create VPU");
        std::shared_ptr<computer::VPU> vpu = std::make_shared<computer::VPU>(vram);
        vpu->setName("VPU");
        printDebug("Create IOCTRL");
        std::shared_ptr<computer::IOController> io = std::make_shared<computer::IOController>();
        io->setName("IOCTRL");
        printDebug("Create HWSTATS");
        hwStats = std::make_shared<computer::HardwareStates>();
        hwStats->setName("HWSTATS");

        printDebug("Add Device HWSTATS");
        addDevice(hwStats, 0x1C00, 0x1C20);
        printDebug("Add Device ROM4K");
        addDevice(rom4k, 0x0000, 0x0FFF);
        printDebug("Add Device ROM2K");
        addDevice(rom2k, 0x1000, 0x17FF);
        printDebug("Add Device RAM1K");
        addDevice(ram1k, 0x1800, 0x1BFF);
        printDebug("Add Device VPU");
        addDevice(vpu, 0x1C18, 0x1C1F);
        printDebug("Add Device IOCTRL");
        addDevice(io, 0x1D00, 0x1D1F);
        printDebug("Add Device VRAM8K");
        addDevice(vram, 0x2000, 0x3FFF);
        printDebug("Add Device RAM16K");
        addDevice(ram16k, 0x4000, 0x7FFF);

        printDebug("load program into ROM4K");
        rom4k = std::static_pointer_cast<computer::ROM>(getDevice("ROM", 0x0000, 0x0FFF));
        rom4k->load(prog);
        io.reset();
    }
    else
    {
        printDebug("Create HWSTATS");
        hwStats = std::make_shared<computer::HardwareStates>();
        printDebug("Add Device HWSTATS");
        addDevice(hwStats, 0x1C00, 0x1C20);
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
    hwStats->connect(device, startAdr, endAdr);
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
            hwStats->disconnect(devices[i], startAdr, endAdr);
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

std::vector<std::shared_ptr<computer::Device>> computer::Computer::getAllDevice()
{
    return std::vector<std::shared_ptr<computer::Device>>(devices);
}

void computer::Computer::connectIODevice(std::shared_ptr<computer::IODevice> device, uint8_t port)
{
    std::shared_ptr<computer::IOController> io = std::static_pointer_cast<computer::IOController>(getDevice("IOCTRL", 0x0000,0xFFFF));
    if(io)
    {
        printDebug("Computer: add io");
        io->addIO(device, port);
    }
}

void computer::Computer::removeIODevvice(uint8_t port)
{
    std::shared_ptr<computer::IOController> io = std::static_pointer_cast<computer::IOController>(getDevice("IOCTRL", 0x0000,0xFFFF));
    if(io)
    {
        printDebug("Computer: remove io");
        io->removeIO(port);
    }
}
