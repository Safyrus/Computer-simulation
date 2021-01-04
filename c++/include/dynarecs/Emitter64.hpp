#ifndef EMITTER86_HPP
#define EMITTER86_HPP

#include "dynarecs/Emitter.hpp"

namespace dynarec
{
    class Emitter64: public dynarec::Emitter
    {
    protected:
        void x64MOV_RtM(x64REG from, uint64_t to, bool mode8 = true);
        void x64MOV_MtR(uint64_t from, x64REG to, bool mode8 = true);
        void x64MOV_RtR(x64REG from, x64REG to, bool mode8 = true, bool extendBase = false, bool extendReg = false);
        void x64MOV_Rimm(x64REG reg, uint64_t val, bool regMode = false);

        void x64ADD_RtR(x64REG dst, x64REG src, bool mode8 = true);
        void x64ADC_RtR(x64REG dst, x64REG src, bool mode8 = true);
        void x64SUB_RtR(x64REG dst, x64REG src, bool mode8 = true);
        void x64SBB_RtR(x64REG dst, x64REG src, bool mode8 = true);
        void x64MUL_R(x64REG src);
        void x64DIV_R(x64REG src);

        void x64SHL(x64REG reg, uint8_t val, bool mode8 = true);
        void x64OR_Rimm(x64REG reg, uint64_t val);
        void x64OR_RtR(x64REG dst, x64REG src, bool mode8 = true, bool extendBase = false, bool extendReg = false);
        void x64AND_Rimm(x64REG reg, uint64_t val);
        void x64AND_RtR(x64REG dst, x64REG src, bool mode8 = true, bool extendBase = false, bool extendReg = false);
        void x64XOR_RtR(x64REG dst, x64REG src, bool mode8 = true, bool extendBase = false, bool extendReg = false);

        void x64CMP(x64REG r1, x64REG r2, bool mode8 = true);

        void x64JE(uint8_t rel);
        void x64JG(uint8_t rel);
        void x64JL(uint8_t rel);
        void x64JC(uint8_t rel);
        void x64JNC(uint8_t rel);

        void x64PushF();
        void x64PopF();

        uint8_t x64ModRM(uint8_t mode, x64REG reg, uint8_t rm);
        void x64RET();
        void x64RAND();

        void cmpToF();

    public:
        Emitter64(std::shared_ptr<computer::CPU> cpu, uint16_t startAdr);
        ~Emitter64();

        void NXT();

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

#endif // EMITTER86_HPP