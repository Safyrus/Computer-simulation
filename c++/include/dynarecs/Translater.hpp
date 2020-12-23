#ifndef TRANSLATER_HPP
#define TRANSLATER_HPP

#include "computer/CPU.hpp"
#include "dynarecs/Emitter.hpp"
#include <memory>
#include <vector>

namespace dynarec
{
    class Translater
    {
    private:
        std::vector<dynarec::Emitter *> blocks;
        std::shared_ptr<computer::CPU> cpu;
        bool running;
        bool rawBus;

        dynarec::Emitter *handlerEndBlock(int ret);
        dynarec::Emitter *getBlock(uint16_t pc);
        void recompile(uint16_t pc);

    public:
        static const uint8_t CODE_RET = 1;
        static const uint8_t CODE_JMP = 2;
        static const uint8_t CODE_NXT = 3;
        static const uint8_t CODE_ERR = 4;

        Translater(std::shared_ptr<computer::CPU> cpu, bool rawBus);
        ~Translater();

        int run(uint16_t pc);
    };
} // namespace dynarec

#endif // TRANSLATER