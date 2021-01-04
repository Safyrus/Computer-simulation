#include "dynarecs/Emitter.hpp"

dynarec::Emitter::Emitter(std::shared_ptr<computer::CPU> cpu, uint16_t startAdr) : buf(startAdr, 0)
{
    this->cpu = cpu;
}

dynarec::Emitter::~Emitter()
{
}

uint16_t dynarec::Emitter::getInsCount()
{
    return buf.getInsCount();
}

uint16_t dynarec::Emitter::getStartAdr()
{
    return buf.getStartAdr();
}

int dynarec::Emitter::execute()
{
    return buf.execute();
}