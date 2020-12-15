#include "dynarecs/Emitter.hpp"
#include "dynarecs/Translater.hpp"

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

void dynarec::Emitter::NOP()
{
    buf.write8(0x90);
    buf.incInsCount();
}

/*void Emitter::RST()
{
}*/

void dynarec::Emitter::OFF()
{
    // uint32_t adr = (uint32_t)&cpu->reg[A];
    // x86MOV_MtR(adr, EAX);
    x86MOV_Rimm(EAX, Translater::CODE_RET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::MOV(REG dst, uint8_t src)
{
    uint32_t adr = (uint32_t)&cpu->reg[dst];
    x86MOV_Rimm(EAX, src);
    x86MOV_RtM(EAX, adr);
    buf.incInsCount();
}

void dynarec::Emitter::MOV(REG dst, REG src)
{
    uint32_t adrD = (uint32_t)&cpu->reg[dst];
    uint32_t adrS = (uint32_t)&cpu->reg[src];
    x86MOV_MtR(adrS, EAX);
    x86MOV_RtM(EAX, adrD);
    buf.incInsCount();
}

void dynarec::Emitter::CMP(REG src, REG val)
{
    //get registers
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter::CMP(uint8_t src, REG val)
{
    //get registers
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter::CMP(REG src, uint8_t val)
{
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    cmpToF();
    buf.incInsCount();
}
void dynarec::Emitter::CMP(uint8_t src, uint8_t val)
{
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter::ADD(REG dst, REG src, REG val)
{
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86ADD(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}
/*
void dynarec::Emitter::ADC(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::SUB(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::SBB(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::MUL(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::DIV(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::MOD(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::AND(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::OR(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::XOR(REG dst, REG src, REG val)
{
}

void dynarec::Emitter::ADD(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::ADC(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::SUB(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::SBB(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::MUL(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::DIV(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::MOD(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::AND(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::OR(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::XOR(REG dst, uint8_t src, REG val)
{
}

void dynarec::Emitter::ADD(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::ADC(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::SUB(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::SBB(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::MUL(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::DIV(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::MOD(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::AND(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::OR(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::XOR(REG dst, REG src, uint8_t val)
{
}

void dynarec::Emitter::ADD(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::ADC(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::SUB(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::SBB(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::MUL(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::DIV(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::MOD(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::AND(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::OR(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::XOR(REG dst, uint8_t src, uint8_t val)
{
}
*/

void dynarec::Emitter::JMP(REG dst, REG src, REG val)
{
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EDX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}

/*
void dynarec::Emitter::JMP(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::JMP(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::JMP(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::JMP(uint8_t dst, REG src, REG val)
{
}
void dynarec::Emitter::JMP(uint8_t dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::JMP(uint8_t dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::JMP(uint8_t dst, uint8_t src, uint8_t val)
{
}

void dynarec::Emitter::GET(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::GET(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::GET(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::GET(REG dst, uint8_t src, uint8_t val)
{
}

void dynarec::Emitter::SET(REG dst, REG src, REG val)
{
}
void dynarec::Emitter::SET(REG dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::SET(REG dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::SET(REG dst, uint8_t src, uint8_t val)
{
}
void dynarec::Emitter::SET(uint8_t dst, REG src, REG val)
{
}
void dynarec::Emitter::SET(uint8_t dst, uint8_t src, REG val)
{
}
void dynarec::Emitter::SET(uint8_t dst, REG src, uint8_t val)
{
}
void dynarec::Emitter::SET(uint8_t dst, uint8_t src, uint8_t val)
{
}
*/