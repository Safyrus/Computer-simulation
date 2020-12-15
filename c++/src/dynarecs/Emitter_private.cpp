#include "dynarecs/Emitter.hpp"

void dynarec::Emitter::x86MOV_RtM(x86REG from, uint32_t to, bool mode8)
{
    buf.write8(0x89 - mode8);
    buf.write8(x86ModRM(0, from, 5));
    buf.write32(to);
}

void dynarec::Emitter::x86MOV_MtR(uint32_t from, x86REG to, bool mode8)
{
    buf.write8(0x8B - mode8);
    buf.write8(x86ModRM(0, to, 5));
    buf.write32(from);
}

void dynarec::Emitter::x86MOV_RtR(x86REG from, x86REG to, bool mode8)
{
    buf.write8(0x89 - mode8);
    buf.write8(x86ModRM(3, from, to));
}

void dynarec::Emitter::x86MOV_Rimm(x86REG reg, uint32_t val)
{
    buf.write8(0xB8 + reg);
    buf.write32(val);
}

void dynarec::Emitter::x86ADD(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x01 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter::x86AND_Rimm(x86REG reg, uint32_t val)
{
    buf.write8(0x81);
    buf.write8(x86ModRM(3, (x86REG)4, reg));
    buf.write32(val);
}

void dynarec::Emitter::x86CMP(x86REG r1, x86REG r2, bool mode8)
{
    buf.write8(0x39 - mode8);
    buf.write8(x86ModRM(3, r2, r1));
}

void dynarec::Emitter::x86SHL(x86REG reg, uint8_t val, bool mode8)
{
    buf.write8(0xC1 - mode8);
    buf.write8(x86ModRM(3, (x86REG)4, reg));
    buf.write8(val);
}

void dynarec::Emitter::x86OR_Rimm(x86REG reg, uint32_t val)
{
    buf.write8(0x81);
    buf.write8(x86ModRM(3, (x86REG)1, reg));
    buf.write32(val);
}

void dynarec::Emitter::x86OR_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x09 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter::x86JE(uint8_t rel)
{
    buf.write8(0x74);
    buf.write8(rel);
}

void dynarec::Emitter::x86JG(uint8_t rel)
{
    buf.write8(0x7F);
    buf.write8(rel);
}

void dynarec::Emitter::x86JL(uint8_t rel)
{
    buf.write8(0x7C);
    buf.write8(rel);
}

void dynarec::Emitter::x86JC(uint8_t rel)
{
    buf.write8(0x72);
    buf.write8(rel);
}

void dynarec::Emitter::x86PushF()
{
    buf.write8(0x9c);
}

void dynarec::Emitter::x86PopF()
{
    buf.write8(0x9d);
}

uint8_t dynarec::Emitter::x86ModRM(uint8_t mode, x86REG reg, uint8_t rm)
{
    return ((mode << 6) & 0xC0) + ((reg << 3) & 0x38) + ((rm)&0x07);
}

void dynarec::Emitter::x86RET()
{
    buf.write8(0xC3);
}

void dynarec::Emitter::cmpToF()
{
    //compare
    //x86AND_Rimm(EAX, 0xff);
    //x86AND_Rimm(ECX, 0xff);
    x86CMP(EAX, ECX);
    //get F
    x86MOV_MtR((uint32_t)&cpu->reg[F], EDX);
    x86MOV_Rimm(EDX, 0x0f); // TODO change to OR

    // greater
    x86JG(0x08);
    x86PushF();
    x86AND_Rimm(EDX, 0xfd);
    x86PopF();
    // equal
    x86JE(0x08);
    x86PushF();
    x86AND_Rimm(EDX, 0xfb);
    x86PopF();
    // lesser
    x86JL(0x06);
    x86AND_Rimm(EDX, 0xf7);
    // carry
    x86ADD(EAX, ECX);
    x86JC(0x06);
    x86AND_Rimm(EDX, 0xfe);

    // update F
    x86MOV_RtM(EDX, (uint32_t)&cpu->reg[F]);
}
