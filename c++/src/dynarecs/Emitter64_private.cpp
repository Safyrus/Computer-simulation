#include "dynarecs/Emitter64.hpp"

void dynarec::Emitter64::x64MOV_RtM(x64REG from, uint64_t to, bool mode8)
{
    //x64MOV_RtR(RAX, R8, false, false, true);
    x64MOV_Rimm(R8, to, true);
    buf.write8(0x41);
    buf.write8(0x89-mode8);
    buf.write8(x64ModRM(0, from, RAX));
    //x64MOV_RtR(R8, RAX, false, true, false);
}

void dynarec::Emitter64::x64MOV_MtR(uint64_t from, x64REG to, bool mode8)
{
    //x64MOV_RtR(RAX, R8, false, false, true);
    x64MOV_Rimm(R8, from, true);
    buf.write8(0x41);
    buf.write8(0x8b-mode8);
    buf.write8(x64ModRM(0, to, RAX));
    //x64MOV_RtR(R8, RAX, false, true, false);
}

void dynarec::Emitter64::x64MOV_RtR(x64REG from, x64REG to, bool mode8, bool extendBase, bool extendReg)
{
    if(!mode8)
    {
        buf.write8(0x48+extendBase+(extendReg*4));
    }
    buf.write8(0x89-mode8);
    buf.write8(x64ModRM(3, from, to));
}

void dynarec::Emitter64::x64MOV_Rimm(x64REG reg, uint64_t val, bool regMode)
{
    buf.write8(0x48+regMode);
    buf.write8(0xB8 + reg);
    buf.write64(val);
}

void dynarec::Emitter64::x64CMP(x64REG r1, x64REG r2, bool mode8)
{
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0x39 - mode8);
    buf.write8(x64ModRM(3, r2, r1));
}


/**************************************************/


void dynarec::Emitter64::x64ADD_RtR(x64REG dst, x64REG src, bool mode8)
{
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0x01 - mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64ADC_RtR(x64REG dst, x64REG src, bool mode8)
{
    buf.write8(0xf9);
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0x11 - mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64SUB_RtR(x64REG dst, x64REG src, bool mode8)
{
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0x29 - mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64SBB_RtR(x64REG dst, x64REG src, bool mode8)
{
    buf.write8(0xf9);
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0x19 - mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64MUL_R(x64REG src)
{
    buf.write8(0x48);
    buf.write8(0xf7);
    buf.write8(x64ModRM(3, (x64REG)4, src));
}

void dynarec::Emitter64::x64DIV_R(x64REG src)
{
    buf.write8(0x48);
    buf.write8(0xf7);
    buf.write8(x64ModRM(3, (x64REG)6, src));
}


/**************************************************/


void dynarec::Emitter64::x64AND_Rimm(x64REG reg, uint64_t val)
{
    x64MOV_Rimm(R8, val, true);
    x64AND_RtR(reg, R8, false, false, true);
}

void dynarec::Emitter64::x64AND_RtR(x64REG dst, x64REG src, bool mode8, bool extendBase, bool extendReg)
{
    if(!mode8)
    {
        buf.write8(0x48+extendBase+(extendReg*4));
    }
    buf.write8(0x21-mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64OR_Rimm(x64REG reg, uint64_t val)
{
    x64MOV_Rimm(R8, val, true);
    x64OR_RtR(reg, R8, false, false, true);
}

void dynarec::Emitter64::x64OR_RtR(x64REG dst, x64REG src, bool mode8, bool extendBase, bool extendReg)
{
    if(!mode8)
    {
        buf.write8(0x48+extendBase+(extendReg*4));
    }
    buf.write8(0x09-mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64XOR_RtR(x64REG dst, x64REG src, bool mode8, bool extendBase, bool extendReg)
{
    if(!mode8)
    {
        buf.write8(0x48+extendBase+(extendReg*4));
    }
    buf.write8(0x31-mode8);
    buf.write8(x64ModRM(3, src, dst));
}

void dynarec::Emitter64::x64SHL(x64REG reg, uint8_t val, bool mode8)
{
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0xC1 - mode8);
    buf.write8(x64ModRM(3, (x64REG)4, reg));
    buf.write8(val);
}

void dynarec::Emitter64::x64SHR(x64REG reg, uint8_t val, bool mode8)
{
    if(!mode8)
    {
        buf.write8(0x48);
    }
    buf.write8(0xC1 - mode8);
    buf.write8(x64ModRM(3, (x64REG)5, reg));
    buf.write8(val);
}

/**************************************************/


void dynarec::Emitter64::x64JE(uint8_t rel)
{
    buf.write8(0x74);
    buf.write8(rel);
}

void dynarec::Emitter64::x64JG(uint8_t rel)
{
    buf.write8(0x7F);
    buf.write8(rel);
}

void dynarec::Emitter64::x64JL(uint8_t rel)
{
    buf.write8(0x7C);
    buf.write8(rel);
}

void dynarec::Emitter64::x64JC(uint8_t rel)
{
    buf.write8(0x72);
    buf.write8(rel);
}

void dynarec::Emitter64::x64JNC(uint8_t rel)
{
    buf.write8(0x73);
    buf.write8(rel);
}


/**************************************************/


void dynarec::Emitter64::x64PushF()
{
    buf.write8(0x9c);
}

void dynarec::Emitter64::x64PopF()
{
    buf.write8(0x9d);
}

void dynarec::Emitter64::x64RET()
{
    buf.write8(0xC3);
}

void dynarec::Emitter64::x64RAND()
{
    x64MOV_Rimm(RAX, 0);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[O]);
    x64MOV_MtR((uint64_t)&cpu->reg[R], RAX, false);
    x64MOV_RtR(RAX, RCX, false);
    x64SHL(RCX, 5, false);
    x64XOR_RtR(RCX, RAX, false);
    x64AND_Rimm(RCX, 0x0020);
    x64SHL(RCX, 4, false);
    x64OR_RtR(RAX, RCX, false);
    x64SHR(RAX, 1, false);
    x64MOV_RtM(RAX, (uint64_t)&cpu->reg[R], false);
}


/**************************************************/


uint8_t dynarec::Emitter64::x64ModRM(uint8_t mode, x64REG reg, uint8_t rm)
{
    return ((mode << 6) & 0xC0) + ((reg << 3) & 0x38) + ((rm)&0x07);
}

void dynarec::Emitter64::cmpToF()
{
    //get F
    x64MOV_Rimm(RDX, 0x0f);
    //compare
    x64CMP(RAX, RCX);

    // greater
    x64JG(0x0f);
    x64PushF();
    x64AND_Rimm(RDX, 0xfd);
    x64PopF();
    // equal
    x64JE(0x0f);
    x64PushF();
    x64AND_Rimm(RDX, 0xfb);
    x64PopF();
    // lesser
    x64JL(0x0f);
    x64PushF();
    x64AND_Rimm(RDX, 0xf7);
    x64PopF();
    // carry
    //x64ADD_RtR(RAX, RCX);
    x64JNC(0x0d);
    x64AND_Rimm(RDX, 0xfe);

    // update F
    x64MOV_RtM(RDX, (uint64_t)&cpu->reg[F]);
}
