#include "computer/CPU.hpp"

#include "dynarecs/Translater.hpp"

#include <vector>

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

#include "global.hpp"

#include "utils/console.hpp"

computer::CPU::CPU(std::shared_ptr<computer::Bus> bus, bool threadWanted) : Device()
{
    printDebug("CPU Creation");
    this->threadWanted = threadWanted;
    type = "CPU";
    name = "S257-01";
    this->bus = bus;
    hz = 0;
    printInstructions = true;
    resetReg();
    for (unsigned int i = 0; i < 1024 * 64; i++)
    {
        rawBus[i] = 0;
    }
}

computer::CPU::~CPU()
{
    printDebug("CPU Destruction");
}

void computer::CPU::run()
{
    translater = std::make_shared<dynarec::Translater>(shared_from_this(), false);
    translater->print = printInstructions;
    if (!threadWanted)
    {
        translater->run(pc);
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
                    if (!hasReset)
                    {
                        reset();
                    }
                    else
                    {
                        resetReg();
                    }
                    hasReset = false;
                    translater->initStep(pc);
                }
                translater->runStep();
            }
            else
            {
                if (start)
                {
                    start = false;
                }
                std::chrono::nanoseconds timeWait(1000000);
                std::this_thread::sleep_for(timeWait);
            }
        }
    }
    translater = nullptr;
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
    if (bus != nullptr)
    {
        bus->reset();
    }
}

void computer::CPU::resetReg()
{
    cycle = 0;
    refreshCycle(0);
    pc = 0;
    for (unsigned int i = 0; i < 16; i++)
    {
        reg[i] = 0;
    }
    reg[R] = 0x100;
}

void computer::CPU::setPwr(bool pwr)
{
    this->pwr = pwr;
    if (bus != nullptr)
    {
        bus->setPwr(pwr);
    }
}

void computer::CPU::pause()
{
    if (translater)
    {
        translater->setPause(!translater->getPause());
    }
}

bool computer::CPU::isPause()
{
    if(translater)
    {
        return translater->getPause();
    }
    return false;
}

void computer::CPU::step()
{
    if(translater)
    {
        return translater->stepOnce();
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

void computer::CPU::refreshCycle(uint64_t)
{
    cycleCPU = this->cycle;
    if (bus)
    {
        bus->refreshCycle(this->cycle);
    }
}

void computer::CPU::doPrintInstructions(bool print)
{
    printInstructions = print;
}

bool computer::CPU::getPrintInstructions()
{
    return printInstructions;
}

void computer::CPU::setBreakpoint(uint16_t adr)
{
    if (translater)
    {
        translater->setBreakpoint(adr);
    }
}

void computer::CPU::removeBreakpoint(uint16_t adr)
{
    if (translater)
    {
        translater->removeBreakpoint(adr);
    }
}

std::vector<uint16_t> computer::CPU::getBreakpoints()
{
    std::vector<uint16_t> breakpoints;
    if (translater)
    {
        breakpoints = translater->getBreakpoints();
    }
    return breakpoints;
}
