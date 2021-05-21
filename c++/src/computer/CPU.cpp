#include "computer/CPU.hpp"

#include "dynarecs/Translater.hpp"

#include <vector>

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::CPU::CPU(std::shared_ptr<computer::Bus> bus) : Device()
{
    threadWanted = false;
    type = "CPU";
    this->bus = bus;
    cycle = 0;
    pc = 0;
    hz = 0;
    for (unsigned int i = 0; i < 16; i++)
    {
        reg[i] = 0;
    }
    for (unsigned int i = 0; i < 1024 * 64; i++)
    {
        rawBus[i] = 0;
    }
}

computer::CPU::CPU(std::shared_ptr<computer::Bus> bus, bool threadWanted) : Device()
{
    this->threadWanted = threadWanted;
    type = "CPU";
    this->bus = bus;
    cycle = 0;
    pc = 0;
    hz = 0;
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
    if (!threadWanted)
    {
        t.run(pc);
    }
    else
    {
        bool start = false;
        while (running)
        {
            if (pwr)
            {
                if (!start || hasReset)
                {
                    start = true;
                    if(!hasReset)
                    {
                        reset();
                    }else
                    {
                        resetReg();
                    }
                    hasReset = false;
                    t.initStep(pc);
                }
                t.runStep();
            }
            else
            {
                if(start)
                {
                    start = false;
                }
                std::chrono::nanoseconds timeWait(1000000);
                std::this_thread::sleep_for(timeWait);
            }
        }
    }
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
    resetReg();
    hasReset = true;
    if(bus != nullptr)
    {
        bus->reset();
    }
}

void computer::CPU::resetReg()
{
    cycle = 0;
    pc = 0;
    for (unsigned int i = 0; i < 16; i++)
    {
        reg[i] = 0;
    }
    reg[R] = rand();
}

void computer::CPU::setPwr(bool pwr)
{
    this->pwr = pwr;
    if(bus != nullptr)
    {
        bus->setPwr(pwr);
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
