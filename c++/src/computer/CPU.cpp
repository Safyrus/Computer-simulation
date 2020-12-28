#include "computer/CPU.hpp"

#include "dynarecs/Translater.hpp"

#include <vector>

computer::CPU::CPU(std::shared_ptr<computer::Bus> bus) : Device()
{
    this->bus = bus;
    cycle = 0;
    pc = 0;
    for (unsigned int i = 0; i < 16; i++)
    {
        reg[i] = 0;
    }
    for (unsigned int i = 0; i < 1024 * 64; i++)
    {
        rawBus[i] = 0;
    }
}

computer::CPU::~CPU()
{
}

void computer::CPU::run()
{
    dynarec::Translater t(shared_from_this(), false);
    t.run(pc);
}

uint8_t computer::CPU::get(uint16_t adr)
{
    return bus->get(adr);
}

void computer::CPU::set(uint16_t adr, uint8_t data)
{
    bus->set(adr, data);
}

void computer::CPU::reset()
{
    cycle = 0;
    pc = 0;
    for (unsigned int i = 0; i < 16; i++)
    {
        reg[i] = 0;
    }
}

uint8_t computer::CPU::getBusData(uint16_t adr)
{
    return rawBus[adr];
}

void computer::CPU::setBusData(uint16_t adr, uint8_t data)
{
    rawBus[adr] = data;
}

void computer::CPU::loadOnBus(uint16_t start, std::vector<uint8_t> data)
{
    for (unsigned int i = 0; i < data.size() && i < 0x10000; i++)
    {
        rawBus[start + i] = data[i];
    }
}
