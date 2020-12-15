#include "computer/CPU.hpp"
#include <vector>

computer::CPU::CPU()
{
    cycle = 0;
    pc = 0;
    for (unsigned int i = 0; i < 16; i++)
    {
        reg[i] = 0;
    }
}

computer::CPU::~CPU()
{
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
    return bus[adr];
}

void computer::CPU::setBusData(uint16_t adr, uint8_t data)
{
    bus[adr] = data;
}

void computer::CPU::loadOnBus(uint16_t start, std::vector<uint8_t> data)
{
    for (unsigned int i = 0; i < data.size() && i < 0x10000; i++)
    {
        bus[start+i] = data[i];
    }
}
