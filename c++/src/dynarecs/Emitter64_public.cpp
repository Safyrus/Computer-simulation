#include "dynarecs/Emitter64.hpp"
#include "dynarecs/Translater.hpp"

#include <iostream>

dynarec::Emitter64::Emitter64(std::shared_ptr<computer::CPU> cpu, uint16_t startAdr) : Emitter(cpu, startAdr)
{

}

dynarec::Emitter64::~Emitter64()
{
}

void dynarec::Emitter64::NXT()
{
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
}


/**************************************************/


void dynarec::Emitter64::NOP()
{
    x64RAND();
    buf.write8(0x90);
    buf.incInsCount();
}

void dynarec::Emitter64::RST()
{
    x64MOV_Rimm(RAX, Translater::CODE_RST);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::OFF()
{
    // uint64_t adr = (uint64_t)&cpu->reg[A];
    // x64MOV_MtR(adr, RAX);
    x64MOV_Rimm(RAX, Translater::CODE_RET);
    x64RET();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::MOV(REG dst, uint8_t src)
{
    x64RAND();
    uint64_t adr = (uint64_t)&cpu->reg[dst];
    x64MOV_Rimm(RAX, src);
    x64MOV_RtM(RAX, adr);
    buf.incInsCount();
}

void dynarec::Emitter64::MOV(REG dst, REG src)
{
    x64RAND();
    uint64_t adrD = (uint64_t)&cpu->reg[dst];
    uint64_t adrS = (uint64_t)&cpu->reg[src];
    x64MOV_MtR(adrS, RAX);
    x64MOV_RtM(RAX, adrD);
    buf.incInsCount();
}

void dynarec::Emitter64::CMP(REG src, REG val)
{
    x64RAND();
    //get registers
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter64::CMP(uint8_t src, REG val)
{
    x64RAND();
    //get registers
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter64::CMP(REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    cmpToF();
    buf.incInsCount();
}
void dynarec::Emitter64::CMP(uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    cmpToF();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::ADD(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64ADD_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::ADC(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64ADC_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SUB(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64SUB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}
void dynarec::Emitter64::SBB(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64SBB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MUL(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64MUL_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::DIV(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64AND_Rimm(RAX, 0xff);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64AND_Rimm(RCX, 0xff);
    //x64MOV_RtR(RAX, EBX);
    //buf.write8(0x99);
    x64DIV_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    //x64MOV_RtM(RDX, (uint64_t)&cpu->reg[D]);
    buf.incInsCount();
}

void dynarec::Emitter64::MOD(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64AND_Rimm(RAX, 0xff);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64AND_Rimm(RCX, 0xff);
    x64DIV_R(RCX);
    x64MOV_RtM(RDX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::AND(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64AND_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::OR(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::XOR(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64XOR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::ADD(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64ADD_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::ADC(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64ADC_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SUB(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64SUB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SBB(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64SBB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MUL(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64MUL_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::DIV(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64AND_Rimm(RCX, 0xff);
    x64DIV_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MOD(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64AND_Rimm(RCX, 0xff);
    x64DIV_R(RCX);
    x64MOV_RtM(RDX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::AND(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64AND_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::OR(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::XOR(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64XOR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::ADD(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64ADD_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::ADC(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64ADC_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SUB(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64SUB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SBB(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64SBB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MUL(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64MUL_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::DIV(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64AND_Rimm(RAX, 0xff);
    x64MOV_Rimm(RCX, val);
    x64DIV_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MOD(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64AND_Rimm(RAX, 0xff);
    x64MOV_Rimm(RCX, val);
    x64DIV_R(RCX);
    x64MOV_RtM(RDX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::AND(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64AND_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::OR(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::XOR(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RAX);
    x64MOV_Rimm(RCX, val);
    x64XOR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::ADD(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64ADD_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::ADC(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64ADC_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SUB(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64SUB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::SBB(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64SBB_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MUL(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64MUL_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::DIV(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64DIV_R(RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::MOD(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RDX, 0);
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64DIV_R(RCX);
    x64MOV_RtM(RDX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::AND(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64AND_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::OR(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter64::XOR(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, src);
    x64MOV_Rimm(RCX, val);
    x64XOR_RtR(RAX, RCX);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::JMP(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RDX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_Rimm(RDX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RDX);
    x64MOV_Rimm(RCX, val);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_Rimm(RDX, src);
    x64MOV_Rimm(RCX, val);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(uint8_t dst, REG src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RDX);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(uint8_t dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_Rimm(RDX, src);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(uint8_t dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_MtR((uint64_t)&cpu->reg[src], RDX);
    x64MOV_Rimm(RCX, val);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::JMP(uint8_t dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64MOV_MtR((uint64_t)&cpu->reg[F], RCX);
    x64CMP(RAX, RCX);
    x64JE(0x0b);
    x64MOV_Rimm(RAX, Translater::CODE_NXT);
    x64RET();
    x64MOV_Rimm(RDX, src);
    x64MOV_Rimm(RCX, val);
    x64MOV_RtR(RDX, RAX, false);
    x64SHL(RAX, 8, false);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_JMP);
    x64RET();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::GET(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RCX, dst);
    x64MOV_RtR(RCX, RAX, false);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_GET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::GET(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RCX, dst);
    x64MOV_RtR(RCX, RAX, false);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, src);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_GET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::GET(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RCX, dst);
    x64MOV_RtR(RCX, RAX, false);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_GET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::GET(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RCX, dst);
    x64MOV_RtR(RCX, RAX, false);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, src);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_GET);
    x64RET();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter64::SET(REG dst, REG src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(REG dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, src);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(REG dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(REG dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_MtR((uint64_t)&cpu->reg[dst], RAX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, src);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(uint8_t dst, REG src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(uint8_t dst, uint8_t src, REG val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, src);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[val], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(uint8_t dst, REG src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64SHL(RAX, 8, false);
    x64MOV_MtR((uint64_t)&cpu->reg[src], RCX);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

void dynarec::Emitter64::SET(uint8_t dst, uint8_t src, uint8_t val)
{
    x64RAND();
    x64MOV_Rimm(RAX, dst);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, src);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64MOV_Rimm(RCX, val);
    x64OR_RtR(RAX, RCX);
    x64SHL(RAX, 8, false);
    x64OR_Rimm(RAX, Translater::CODE_SET);
    x64RET();
    buf.incInsCount();
}

