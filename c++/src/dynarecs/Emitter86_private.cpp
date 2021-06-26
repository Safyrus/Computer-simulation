#include "dynarecs/Emitter86.hpp"

void dynarec::Emitter86::x86MOV_RtM(x86REG from, uint32_t to, bool mode8)
{
    buf.write8(0x89 - mode8);
    buf.write8(x86ModRM(0, from, 5));
    buf.write32(to);
}

void dynarec::Emitter86::x86MOV_MtR(uint32_t from, x86REG to, bool mode8)
{
    buf.write8(0x8B - mode8);
    buf.write8(x86ModRM(0, to, 5));
    buf.write32(from);
}

void dynarec::Emitter86::x86MOV_RtR(x86REG from, x86REG to, bool mode8)
{
    buf.write8(0x89 - mode8);
    buf.write8(x86ModRM(3, from, to));
}

void dynarec::Emitter86::x86MOV_Rimm(x86REG reg, uint32_t val)
{
    buf.write8(0xB8 + reg);
    buf.write32(val);
}

void dynarec::Emitter86::x86CMP(x86REG r1, x86REG r2, bool mode8)
{
    buf.write8(0x39 - mode8);
    buf.write8(x86ModRM(3, r2, r1));
}

/**************************************************/

void dynarec::Emitter86::x86ADD_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x01 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter86::x86ADC_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0xf9);
    buf.write8(0x11 - mode8);
    buf.write8(x86ModRM(3, src, dst));
    //buf.write8(0xf8);
}

void dynarec::Emitter86::x86SUB_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x29 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter86::x86SBB_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0xf9);
    buf.write8(0x19 - mode8);
    buf.write8(x86ModRM(3, src, dst));
    //buf.write8(0xf8);
}

void dynarec::Emitter86::x86MUL_R(x86REG src)
{
    buf.write8(0xf7);
    buf.write8(x86ModRM(3, (x86REG)4, src));
}

void dynarec::Emitter86::x86DIV_R(x86REG src)
{
    buf.write8(0xf7);
    buf.write8(x86ModRM(3, (x86REG)6, src));
}

/**************************************************/

void dynarec::Emitter86::x86AND_Rimm(x86REG reg, uint32_t val)
{
    buf.write8(0x81);
    buf.write8(x86ModRM(3, (x86REG)4, reg));
    buf.write32(val);
}

void dynarec::Emitter86::x86AND_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x21 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter86::x86OR_Rimm(x86REG reg, uint32_t val)
{
    buf.write8(0x81);
    buf.write8(x86ModRM(3, (x86REG)1, reg));
    buf.write32(val);
}

void dynarec::Emitter86::x86OR_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x09 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter86::x86XOR_RtR(x86REG dst, x86REG src, bool mode8)
{
    buf.write8(0x31 - mode8);
    buf.write8(x86ModRM(3, src, dst));
}

void dynarec::Emitter86::x86SHL(x86REG reg, uint8_t val, bool mode8)
{
    buf.write8(0xC1 - mode8);
    buf.write8(x86ModRM(3, (x86REG)4, reg));
    buf.write8(val);
}

void dynarec::Emitter86::x86SHR(x86REG reg, uint8_t val, bool mode8)
{
    buf.write8(0xC1 - mode8);
    buf.write8(x86ModRM(3, (x86REG)5, reg));
    buf.write8(val);
}

/**************************************************/

void dynarec::Emitter86::x86JE(uint8_t rel)
{
    buf.write8(0x74);
    buf.write8(rel);
}

void dynarec::Emitter86::x86JG(uint8_t rel)
{
    buf.write8(0x7F);
    buf.write8(rel);
}

void dynarec::Emitter86::x86JL(uint8_t rel)
{
    buf.write8(0x7C);
    buf.write8(rel);
}

void dynarec::Emitter86::x86JC(uint8_t rel)
{
    buf.write8(0x72);
    buf.write8(rel);
}

void dynarec::Emitter86::x86JNC(uint8_t rel)
{
    buf.write8(0x73);
    buf.write8(rel);
}

/**************************************************/

void dynarec::Emitter86::x86PushF()
{
    buf.write8(0x9c);
}

void dynarec::Emitter86::x86PopF()
{
    buf.write8(0x9d);
}

void dynarec::Emitter86::x86RET()
{
    buf.write8(0xC3);
}

void dynarec::Emitter86::x86RAND()
{
    x86MOV_Rimm(EAX, 0);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[O]);
    x86MOV_MtR((uint32_t)&cpu->reg[R], EAX, false);
    x86MOV_RtR(EAX, ECX, false);
    x86SHL(ECX, 5, false);
    x86XOR_RtR(ECX, EAX, false);
    x86AND_Rimm(ECX, 0x0020);
    x86SHL(ECX, 4, false);
    x86OR_RtR(EAX, ECX, false);
    x86SHR(EAX, 1, false);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[R]);
    x86SHR(EAX, 8, false);
    x86MOV_RtM(EAX, (uint32_t)&cpu->reg[R] + 1);
}

/**************************************************/

uint8_t dynarec::Emitter86::x86ModRM(uint8_t mode, x86REG reg, uint8_t rm)
{
    return ((mode << 6) & 0xC0) + ((reg << 3) & 0x38) + ((rm)&0x07);
}

void dynarec::Emitter86::cmpToF()
{
    //get F
    //x86MOV_MtR((uint32_t)&cpu->reg[F], EDX);
    x86MOV_Rimm(EDX, 0x0f); // TODO change to OR
    //compare
    //x86AND_Rimm(EAX, 0xff);
    //x86AND_Rimm(ECX, 0xff);
    x86CMP(EAX, ECX);

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
    x86JL(0x08);
    x86PushF();
    x86AND_Rimm(EDX, 0xf7);
    x86PopF();
    // carry
    //x86ADD_RtR(EAX, ECX);
    x86JNC(0x06);
    x86AND_Rimm(EDX, 0xfe);

    // update F
    x86MOV_RtM(EDX, (uint32_t)&cpu->reg[F]);
}
