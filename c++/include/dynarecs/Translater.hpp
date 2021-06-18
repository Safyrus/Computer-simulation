#ifndef TRANSLATER_HPP
#define TRANSLATER_HPP

#include "computer/CPU.hpp"
#include "dynarecs/Emitter.hpp"
#include <memory>
#include <vector>
#include <chrono>

namespace dynarec
{
    class Translater
    {
    private:
        std::vector<dynarec::Emitter *> blocks;
        std::shared_ptr<computer::CPU> cpu;
        bool running;
        bool rawBus;
        std::chrono::steady_clock::time_point startTime;

        dynarec::Emitter *handlerEndBlock(int ret);
        dynarec::Emitter *getBlock(uint16_t pc);
        void recompile(uint16_t pc);
        void deleteBlocks();
        void deleteBlocks(uint16_t adr);
        void waitInst();

        dynarec::Emitter* e;
        uint32_t lastHz;

        const uint16_t MAX_BLOCK_SIZE = 256;
        uint16_t blockSize;

    public:
        static const uint8_t CODE_RET = 1;
        static const uint8_t CODE_JMP = 2;
        static const uint8_t CODE_NXT = 3;
        static const uint8_t CODE_ERR = 4;
        static const uint8_t CODE_SET = 5;
        static const uint8_t CODE_GET = 6;
        static const uint8_t CODE_RST = 7;

        bool print;

        Translater(std::shared_ptr<computer::CPU> cpu, bool rawBus);
        ~Translater();

        int run(uint16_t pc);
        void initStep(uint16_t pc);
        int runStep();
        void printCPUState();
    };
} // namespace dynarec

#endif // TRANSLATER