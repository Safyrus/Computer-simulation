#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "dynarecs/Buffer.hpp"
#include "computer/CPU.hpp"
#include "computer/registers.h"

#include <memory>

namespace dynarec
{
    class Emitter
    {
    private:
        dynarec::Buffer buf;
        std::shared_ptr<computer::CPU> cpu;

        void x86MOV_RtM(x86REG from, uint32_t to, bool mode8 = true);
        void x86MOV_MtR(uint32_t from, x86REG to, bool mode8 = true);
        void x86MOV_RtR(x86REG from, x86REG to, bool mode8 = true);
        void x86MOV_Rimm(x86REG reg, uint32_t val);

        void x86ADD_RtR(x86REG dst, x86REG src, bool mode8 = true);
        void x86ADC_RtR(x86REG dst, x86REG src, bool mode8 = true);
        void x86SUB_RtR(x86REG dst, x86REG src, bool mode8 = true);
        void x86SBB_RtR(x86REG dst, x86REG src, bool mode8 = true);
        void x86MUL_R(x86REG src);
        void x86DIV_R(x86REG src);

        void x86SHL(x86REG reg, uint8_t val, bool mode8 = true);
        void x86OR_Rimm(x86REG reg, uint32_t val);
        void x86OR_RtR(x86REG dst, x86REG src, bool mode8 = true);
        void x86AND_Rimm(x86REG reg, uint32_t val);
        void x86AND_RtR(x86REG dst, x86REG src, bool mode8 = true);
        void x86XOR_RtR(x86REG dst, x86REG src, bool mode8 = true);

        void x86CMP(x86REG r1, x86REG r2, bool mode8 = true);

        void x86JE(uint8_t rel);
        void x86JG(uint8_t rel);
        void x86JL(uint8_t rel);
        void x86JC(uint8_t rel);
        void x86JNC(uint8_t rel);

        void x86PushF();
        void x86PopF();

        uint8_t x86ModRM(uint8_t mode, x86REG reg, uint8_t rm);
        void x86RET();
        void x86RAND();

        void cmpToF();

    public:
        Emitter(std::shared_ptr<computer::CPU> cpu, uint16_t startAdr);
        ~Emitter();

        int execute();
        uint16_t getInsCount();
        uint16_t getStartAdr();

        void NOP();
        void RST();
        void OFF();

        void MOV(REG dst, uint8_t src);
        void MOV(REG dst, REG src);

        void CMP(REG src, REG val);
        void CMP(uint8_t src, REG val);
        void CMP(REG src, uint8_t val);
        void CMP(uint8_t src, uint8_t val);

        void ADD(REG dst, REG src, REG val);
        void ADC(REG dst, REG src, REG val);
        void SUB(REG dst, REG src, REG val);
        void SBB(REG dst, REG src, REG val);
        void MUL(REG dst, REG src, REG val);
        void DIV(REG dst, REG src, REG val);
        void MOD(REG dst, REG src, REG val);
        void AND(REG dst, REG src, REG val);
        void OR(REG dst, REG src, REG val);
        void XOR(REG dst, REG src, REG val);

        void ADD(REG dst, uint8_t src, REG val);
        void ADC(REG dst, uint8_t src, REG val);
        void SUB(REG dst, uint8_t src, REG val);
        void SBB(REG dst, uint8_t src, REG val);
        void MUL(REG dst, uint8_t src, REG val);
        void DIV(REG dst, uint8_t src, REG val);
        void MOD(REG dst, uint8_t src, REG val);
        void AND(REG dst, uint8_t src, REG val);
        void OR(REG dst, uint8_t src, REG val);
        void XOR(REG dst, uint8_t src, REG val);

        void ADD(REG dst, REG src, uint8_t val);
        void ADC(REG dst, REG src, uint8_t val);
        void SUB(REG dst, REG src, uint8_t val);
        void SBB(REG dst, REG src, uint8_t val);
        void MUL(REG dst, REG src, uint8_t val);
        void DIV(REG dst, REG src, uint8_t val);
        void MOD(REG dst, REG src, uint8_t val);
        void AND(REG dst, REG src, uint8_t val);
        void OR(REG dst, REG src, uint8_t val);
        void XOR(REG dst, REG src, uint8_t val);

        void ADD(REG dst, uint8_t src, uint8_t val);
        void ADC(REG dst, uint8_t src, uint8_t val);
        void SUB(REG dst, uint8_t src, uint8_t val);
        void SBB(REG dst, uint8_t src, uint8_t val);
        void MUL(REG dst, uint8_t src, uint8_t val);
        void DIV(REG dst, uint8_t src, uint8_t val);
        void MOD(REG dst, uint8_t src, uint8_t val);
        void AND(REG dst, uint8_t src, uint8_t val);
        void OR(REG dst, uint8_t src, uint8_t val);
        void XOR(REG dst, uint8_t src, uint8_t val);

        void JMP(REG dst, REG src, REG val);
        void JMP(REG dst, uint8_t src, REG val);
        void JMP(REG dst, REG src, uint8_t val);
        void JMP(REG dst, uint8_t src, uint8_t val);
        void JMP(uint8_t dst, REG src, REG val);
        void JMP(uint8_t dst, uint8_t src, REG val);
        void JMP(uint8_t dst, REG src, uint8_t val);
        void JMP(uint8_t dst, uint8_t src, uint8_t val);

        void GET(REG dst, REG src, REG val);
        void GET(REG dst, uint8_t src, REG val);
        void GET(REG dst, REG src, uint8_t val);
        void GET(REG dst, uint8_t src, uint8_t val);

        void SET(REG dst, REG src, REG val);
        void SET(REG dst, uint8_t src, REG val);
        void SET(REG dst, REG src, uint8_t val);
        void SET(REG dst, uint8_t src, uint8_t val);
        void SET(uint8_t dst, REG src, REG val);
        void SET(uint8_t dst, uint8_t src, REG val);
        void SET(uint8_t dst, REG src, uint8_t val);
        void SET(uint8_t dst, uint8_t src, uint8_t val);
    };
} // namespace dynarec

#endif // EMITTER_HPP