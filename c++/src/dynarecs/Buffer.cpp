#include "dynarecs/Buffer.hpp"

dynarec::Buffer::Buffer(uint16_t startAdr, uint16_t insCount)
{
    this->startAdr = startAdr;
    this->insCount = insCount;
}

dynarec::Buffer::~Buffer()
{
}

uint8_t *dynarec::Buffer::getPtr()
{
    if (mem.size() > 0)
    {
        return &mem[0];
    }
    return NULL;
}

unsigned int dynarec::Buffer::getSize()
{
    return mem.size();
}

void dynarec::Buffer::write8(uint8_t data)
{
    mem.push_back(data);
}

void dynarec::Buffer::write32(uint32_t data)
{
    mem.push_back(data);
    mem.push_back(data >> 8);
    mem.push_back(data >> 16);
    mem.push_back(data >> 24);
}

void dynarec::Buffer::write64(uint64_t data)
{
    mem.push_back(data);
    mem.push_back(data >> 8);
    mem.push_back(data >> 16);
    mem.push_back(data >> 24);
    mem.push_back(data >> 32);
    mem.push_back(data >> 40);
    mem.push_back(data >> 48);
    mem.push_back(data >> 56);
}

int dynarec::Buffer::execute()
{
    int (*func)(void) = (int (*)(void)) & mem[0];
    return func();
}

uint16_t dynarec::Buffer::getStartAdr()
{
    return startAdr;
}

uint16_t dynarec::Buffer::getInsCount()
{
    return insCount;
}

void dynarec::Buffer::setInsCount(uint16_t n)
{
    insCount = n;
}

void dynarec::Buffer::incInsCount()
{
    insCount++;
}
