#ifndef VRAM_HPP
#define VRAM_HPP

#include <memory>

#include "computer/Device.hpp"
#include "computer/RAM.hpp"

namespace computer
{
    class VRAM : public computer::RAM
    {
    private:
        bool lock;

    public:
        VRAM();
        VRAM(uint16_t size);
        VRAM(uint16_t size, uint32_t hz);
        ~VRAM();

        void reset();
        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);

        void setLock(bool lock);
        uint8_t unlockGet(uint16_t adr);
        void unlockSet(uint16_t adr, uint8_t data);
    };
} // namespace computer

#endif // VRAM_HPP