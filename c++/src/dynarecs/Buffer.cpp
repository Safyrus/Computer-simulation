#include "dynarecs/Buffer.hpp"

#ifndef _WIN32
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#endif

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
#ifndef _WIN32
    unsigned char mya[mem.size()];
    std::copy(mem.begin(), mem.end(), mya);
    void *addr = (void*)((unsigned long)mya & ((0UL - 1UL) ^ 0xfff));/*get memory page*/
    int ans = mprotect(addr, 1, PROT_READ|PROT_WRITE|PROT_EXEC);/*set page attributes*/
    if (ans)
    {
        perror("mprotect");
        exit(EXIT_FAILURE);
    }

    int (*func)(void) = ((int(*)(void))mya);
#else
    int (*func)(void) = (int (*)(void)) & mem[0];
#endif
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
