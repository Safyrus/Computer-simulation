#include "computer/VRAM.hpp"
#include "utils/console.hpp"

computer::VRAM::VRAM() : RAM()
{
    type = "VRAM";
    name = "VRAM";
    lock = false;
}

computer::VRAM::VRAM(uint16_t size) : RAM(size)
{
    type = "VRAM";
    name = "VRAM";
    lock = false;
}

computer::VRAM::VRAM(uint16_t size, uint32_t hz) : RAM(size, hz)
{
    type = "VRAM";
    name = "VRAM";
    lock = false;
}

computer::VRAM::~VRAM()
{
}

void computer::VRAM::reset()
{
    RAM::reset();
}

void computer::VRAM::run()
{
    RAM::run();
}

void computer::VRAM::set(uint16_t adr, uint8_t data)
{
    if (!lock)
    {
        RAM::set(adr, data);
    }
    else
    {
        printDebug("VRAM: can't set when lock");
    }
}

uint8_t computer::VRAM::get(uint16_t adr)
{
    if (!lock)
    {
        return RAM::get(adr);
    }
    else
    {
        return 0;
    }
}

void computer::VRAM::setLock(bool lock)
{
    this->lock = lock;
}

uint8_t computer::VRAM::unlockGet(uint16_t adr)
{
    return RAM::get(adr);
}

void computer::VRAM::unlockSet(uint16_t adr, uint8_t data)
{
    RAM::set(adr, data);
}
