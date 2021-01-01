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


/**************************************************/


void dynarec::Emitter::NOP()
{
    x86RAND();
    buf.write8(0x90);
    buf.incInsCount();
}

void dynarec::Emitter::RST()
{
    x86MOV_Rimm(EAX, Translater::CODE_RST);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::OFF()
{
    // uint32_t adr = (uint32_t)&cpu->reg[A];
    // x86MOV_MtR(adr, EAX);
    x86MOV_Rimm(EAX, Translater::CODE_RET);
    x86RET();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::MOV(REG dst, uint8_t src)
{
    x86RAND();
    uint32_t adr = (uint32_t)&cpu->reg[dst];
    x86MOV_Rimm(EAX, src);
    x86MOV_RtM(EAX, adr);
    buf.incInsCount();
}

void dynarec::Emitter::MOV(REG dst, REG src)
{
    x86RAND();
    uint32_t adrD = (uint32_t)&cpu->reg[dst];
    uint32_t adrS = (uint32_t)&cpu->reg[src];
    x86MOV_MtR(adrS, EAX);
    x86MOV_RtM(EAX, adrD);
    buf.incInsCount();
}

void dynarec::Emitter::CMP(REG src, REG val)
{
    x86RAND();
    //get registers
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter::CMP(uint8_t src, REG val)
{
    x86RAND();
    //get registers
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    cmpToF();
    buf.incInsCount();
}

void dynarec::Emitter::CMP(REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    cmpToF();
    buf.incInsCount();
}
void dynarec::Emitter::CMP(uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    cmpToF();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::ADD(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86ADD_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::ADC(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86ADC_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SUB(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86SUB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}
void dynarec::Emitter::SBB(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86SBB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MUL(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86MUL_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::DIV(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86AND_Rimm(EAX, 0xff);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86AND_Rimm(ECX, 0xff);
    //x86MOV_RtR(EAX, EBX);
    //buf.write8(0x99);
    x86DIV_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    //x86MOV_RtM(EDX, (uint32_t)&cpu->reg[D]);
    buf.incInsCount();
}

void dynarec::Emitter::MOD(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86AND_Rimm(EAX, 0xff);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86AND_Rimm(ECX, 0xff);
    x86DIV_R(ECX);
    x86MOV_RtM(EDX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::AND(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86AND_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::OR(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::XOR(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86XOR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::ADD(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86ADD_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::ADC(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86ADC_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SUB(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86SUB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SBB(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86SBB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MUL(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86MUL_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::DIV(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86AND_Rimm(ECX, 0xff);
    x86DIV_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MOD(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86AND_Rimm(ECX, 0xff);
    x86DIV_R(ECX);
    x86MOV_RtM(EDX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::AND(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86AND_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::OR(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::XOR(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86XOR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::ADD(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86ADD_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::ADC(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86ADC_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SUB(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86SUB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SBB(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86SBB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MUL(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86MUL_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::DIV(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86AND_Rimm(EAX, 0xff);
    x86MOV_Rimm(ECX, val);
    x86DIV_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MOD(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86AND_Rimm(EAX, 0xff);
    x86MOV_Rimm(ECX, val);
    x86DIV_R(ECX);
    x86MOV_RtM(EDX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::AND(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86AND_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::OR(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::XOR(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EAX);
    x86MOV_Rimm(ECX, val);
    x86XOR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::ADD(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86ADD_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::ADC(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86ADC_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SUB(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86SUB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::SBB(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86SBB_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MUL(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86MUL_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::DIV(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86DIV_R(ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::MOD(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EDX, 0);
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86DIV_R(ECX);
    x86MOV_RtM(EDX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::AND(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86AND_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::OR(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}

void dynarec::Emitter::XOR(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, src);
    x86MOV_Rimm(ECX, val);
    x86XOR_RtR(EAX, ECX);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[dst]);
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::JMP(REG dst, REG src, REG val)
{
    x86RAND();
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

void dynarec::Emitter::JMP(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_Rimm(EDX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::JMP(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EDX);
    x86MOV_Rimm(ECX, val);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::JMP(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_Rimm(EDX, src);
    x86MOV_Rimm(ECX, val);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::JMP(uint8_t dst, REG src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
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

void dynarec::Emitter::JMP(uint8_t dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_Rimm(EDX, src);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::JMP(uint8_t dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_MtR((uint32_t)&cpu->reg[src], EDX);
    x86MOV_Rimm(ECX, val);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::JMP(uint8_t dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86MOV_MtR((uint32_t)&cpu->reg[F], ECX);
    x86CMP(EAX, ECX);
    x86JE(0x06);
    x86MOV_Rimm(EAX, Translater::CODE_NXT);
    x86RET();
    x86MOV_Rimm(EDX, src);
    x86MOV_Rimm(ECX, val);
    x86MOV_RtR(EDX, EAX, false);
    x86SHL(EAX, 8, false);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_JMP);
    x86RET();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::GET(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_Rimm(ECX, dst);
    x86MOV_RtR(ECX, EAX, false);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[src], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_GET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::GET(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(ECX, dst);
    x86MOV_RtR(ECX, EAX, false);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, src);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_GET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::GET(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(ECX, dst);
    x86MOV_RtR(ECX, EAX, false);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[src], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_GET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::GET(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(ECX, dst);
    x86MOV_RtR(ECX, EAX, false);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, src);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_GET);
    x86RET();
    buf.incInsCount();
}


/**************************************************/


void dynarec::Emitter::SET(REG dst, REG src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[src], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(REG dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, src);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(REG dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[src], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(REG dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_MtR((uint32_t)&cpu->reg[dst], EAX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, src);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(uint8_t dst, REG src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[src], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(uint8_t dst, uint8_t src, REG val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, src);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[val], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(uint8_t dst, REG src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86SHL(EAX, 8, false);
    x86MOV_MtR((uint32_t)&cpu->reg[src], ECX);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

void dynarec::Emitter::SET(uint8_t dst, uint8_t src, uint8_t val)
{
    x86RAND();
    x86MOV_Rimm(EAX, dst);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, src);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86MOV_Rimm(ECX, val);
    x86OR_RtR(EAX, ECX);
    x86SHL(EAX, 8, false);
    x86OR_Rimm(EAX, Translater::CODE_SET);
    x86RET();
    buf.incInsCount();
}

