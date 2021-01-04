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
    protected:
        dynarec::Buffer buf;
        std::shared_ptr<computer::CPU> cpu;

    public:
        Emitter(std::shared_ptr<computer::CPU> cpu, uint16_t startAdr);
        virtual ~Emitter();

        int execute();
        uint16_t getInsCount();
        uint16_t getStartAdr();

        virtual void NXT() = 0;

        virtual void NOP() = 0;
        virtual void RST() = 0;
        virtual void OFF() = 0;

        virtual void MOV(REG dst, uint8_t src) = 0;
        virtual void MOV(REG dst, REG src) = 0;

        virtual void CMP(REG src, REG val) = 0;
        virtual void CMP(uint8_t src, REG val) = 0;
        virtual void CMP(REG src, uint8_t val) = 0;
        virtual void CMP(uint8_t src, uint8_t val) = 0;

        virtual void ADD(REG dst, REG src, REG val) = 0;
        virtual void ADC(REG dst, REG src, REG val) = 0;
        virtual void SUB(REG dst, REG src, REG val) = 0;
        virtual void SBB(REG dst, REG src, REG val) = 0;
        virtual void MUL(REG dst, REG src, REG val) = 0;
        virtual void DIV(REG dst, REG src, REG val) = 0;
        virtual void MOD(REG dst, REG src, REG val) = 0;
        virtual void AND(REG dst, REG src, REG val) = 0;
        virtual void OR(REG dst, REG src, REG val) = 0;
        virtual void XOR(REG dst, REG src, REG val) = 0;

        virtual void ADD(REG dst, uint8_t src, REG val) = 0;
        virtual void ADC(REG dst, uint8_t src, REG val) = 0;
        virtual void SUB(REG dst, uint8_t src, REG val) = 0;
        virtual void SBB(REG dst, uint8_t src, REG val) = 0;
        virtual void MUL(REG dst, uint8_t src, REG val) = 0;
        virtual void DIV(REG dst, uint8_t src, REG val) = 0;
        virtual void MOD(REG dst, uint8_t src, REG val) = 0;
        virtual void AND(REG dst, uint8_t src, REG val) = 0;
        virtual void OR(REG dst, uint8_t src, REG val) = 0;
        virtual void XOR(REG dst, uint8_t src, REG val) = 0;

        virtual void ADD(REG dst, REG src, uint8_t val) = 0;
        virtual void ADC(REG dst, REG src, uint8_t val) = 0;
        virtual void SUB(REG dst, REG src, uint8_t val) = 0;
        virtual void SBB(REG dst, REG src, uint8_t val) = 0;
        virtual void MUL(REG dst, REG src, uint8_t val) = 0;
        virtual void DIV(REG dst, REG src, uint8_t val) = 0;
        virtual void MOD(REG dst, REG src, uint8_t val) = 0;
        virtual void AND(REG dst, REG src, uint8_t val) = 0;
        virtual void OR(REG dst, REG src, uint8_t val) = 0;
        virtual void XOR(REG dst, REG src, uint8_t val) = 0;

        virtual void ADD(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void ADC(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void SUB(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void SBB(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void MUL(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void DIV(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void MOD(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void AND(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void OR(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void XOR(REG dst, uint8_t src, uint8_t val) = 0;

        virtual void JMP(REG dst, REG src, REG val) = 0;
        virtual void JMP(REG dst, uint8_t src, REG val) = 0;
        virtual void JMP(REG dst, REG src, uint8_t val) = 0;
        virtual void JMP(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void JMP(uint8_t dst, REG src, REG val) = 0;
        virtual void JMP(uint8_t dst, uint8_t src, REG val) = 0;
        virtual void JMP(uint8_t dst, REG src, uint8_t val) = 0;
        virtual void JMP(uint8_t dst, uint8_t src, uint8_t val) = 0;

        virtual void GET(REG dst, REG src, REG val) = 0;
        virtual void GET(REG dst, uint8_t src, REG val) = 0;
        virtual void GET(REG dst, REG src, uint8_t val) = 0;
        virtual void GET(REG dst, uint8_t src, uint8_t val) = 0;

        virtual void SET(REG dst, REG src, REG val) = 0;
        virtual void SET(REG dst, uint8_t src, REG val) = 0;
        virtual void SET(REG dst, REG src, uint8_t val) = 0;
        virtual void SET(REG dst, uint8_t src, uint8_t val) = 0;
        virtual void SET(uint8_t dst, REG src, REG val) = 0;
        virtual void SET(uint8_t dst, uint8_t src, REG val) = 0;
        virtual void SET(uint8_t dst, REG src, uint8_t val) = 0;
        virtual void SET(uint8_t dst, uint8_t src, uint8_t val) = 0;
    };
} // namespace dynarec

#endif // EMITTER_HPP