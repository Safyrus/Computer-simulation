#ifndef VPU_HPP
#define VPU_HPP

#include "computer/Device.hpp"
#include "computer/VRAM.hpp"

#include <memory>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

namespace computer
{
    class VPU : public computer::Device
    {
    private:
        const uint32_t PALETTE_16[16] = {
            0x000000FF, 0xAA00AAFF, 0xFF55FFFF, 0xFF0055FF,
            0x000055FF, 0x555555FF, 0xAA0055FF, 0xFFAA00FF,
            0x0055AAFF, 0xAAAAFFFF, 0xAAAAAAFF, 0xFFFF55FF,
            0x00AAFFFF, 0x005500FF, 0x55FF55FF, 0xFFFFFFFF};

        const uint8_t CHAR_MAP[256 * 3] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x04, 0x44, 0x04, 0x0A, 0xA0, 0x00, 0x0A, 0xEA, 0xEA, 0x0E, 0xCE, 0x6E, 0x0A, 0x24, 0x8A, 0x06, 0xA4, 0xAD, 0x08, 0x80, 0x00, 0x04, 0x88, 0x84, 0x04, 0x22, 0x24, 0x0A, 0x4A, 0x00, 0x00, 0x4E, 0x40, 0x00, 0x00, 0x48, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x04, 0x02, 0x24, 0x88,
            0x0E, 0xAA, 0xAE, 0x02, 0x22, 0x22, 0x0E, 0x2E, 0x8E, 0x0E, 0x2E, 0x2E, 0x0A, 0xAE, 0x22, 0x0E, 0x8E, 0x2E, 0x0E, 0x8E, 0xAE, 0x0E, 0x22, 0x22, 0x0E, 0xAE, 0xAE, 0x0E, 0xAE, 0x2E, 0x00, 0x04, 0x04, 0x00, 0x40, 0x48, 0x02, 0x48, 0x42, 0x00, 0xE0, 0xE0, 0x08, 0x42, 0x48, 0x0E, 0x26, 0x04,
            0x04, 0xAA, 0x86, 0x0E, 0xAE, 0xAA, 0x0E, 0xAC, 0xAE, 0x0E, 0x88, 0x8E, 0x0C, 0xAA, 0xAC, 0x0E, 0x8E, 0x8E, 0x0E, 0x8E, 0x88, 0x0E, 0x8A, 0xAE, 0x0A, 0xAE, 0xAA, 0x04, 0x04, 0x44, 0x0E, 0x44, 0x4C, 0x0A, 0xAC, 0xAA, 0x08, 0x88, 0x8E, 0x0A, 0xEE, 0xAA, 0x0C, 0xAA, 0xAA, 0x06, 0xAA, 0xAC,
            0x0E, 0xAE, 0x88, 0x0E, 0xAA, 0xC6, 0x0E, 0xAC, 0xAA, 0x0E, 0x84, 0x2E, 0x0E, 0x44, 0x44, 0x0A, 0xAA, 0xAE, 0x0A, 0xAA, 0xA4, 0x0A, 0xAE, 0xEA, 0x0A, 0xA4, 0xAA, 0x0A, 0xA4, 0x44, 0x0E, 0x24, 0x8E, 0x0C, 0x88, 0x8C, 0x08, 0x84, 0x22, 0x06, 0x22, 0x26, 0x04, 0xA0, 0x00, 0x00, 0x00, 0x0E,
            0x08, 0x40, 0x00, 0x00, 0x4A, 0xEA, 0x00, 0xCC, 0xAE, 0x00, 0x68, 0x86, 0x00, 0xCA, 0xAC, 0x00, 0xEC, 0x8E, 0x00, 0xE8, 0xC8, 0x00, 0x68, 0xA6, 0x00, 0xAA, 0xEA, 0x00, 0x40, 0x44, 0x00, 0xE4, 0x4C, 0x00, 0xAA, 0xCA, 0x00, 0x88, 0x8E, 0x00, 0xEE, 0xAA, 0x00, 0xCA, 0xAA, 0x00, 0x6A, 0xAC,
            0x00, 0xCA, 0xC8, 0x00, 0xEA, 0xC6, 0x00, 0xEA, 0xCA, 0x00, 0xEC, 0x6E, 0x00, 0xE4, 0x44, 0x00, 0xAA, 0xAE, 0x00, 0xAA, 0xA4, 0x00, 0xAA, 0xEE, 0x00, 0xAA, 0x4A, 0x00, 0xAA, 0x44, 0x00, 0xE6, 0xCE, 0x02, 0x4C, 0x42, 0x04, 0x44, 0x44, 0x08, 0x46, 0x48, 0x00, 0x2E, 0x80, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xEC, 0xA1, 0x00, 0x73, 0x58, 0x00, 0x00, 0x1A, 0xCE, 0x00, 0x85, 0x37, 0x06, 0x99, 0x60, 0x00, 0xF9, 0x00, 0x69, 0x1F, 0xFF, 0x06, 0x9F, 0xFF, 0x00, 0x1A, 0x40, 0xF9, 0xBD, 0x9F, 0xF9, 0x99, 0x9F, 0x0C, 0xA9, 0x9F, 0x0C, 0xFF, 0xF0, 0x03, 0x2E, 0xE0, 0x05, 0xB7, 0x31, 0x0C, 0xEC, 0x80,
            0x09, 0x90, 0x96, 0x09, 0x90, 0x69, 0x09, 0x90, 0x0F, 0x09, 0x90, 0x6F, 0x09, 0x90, 0xF6, 0x09, 0x90, 0x86, 0x00, 0x66, 0x00, 0x06, 0xFF, 0x60, 0x00, 0x69, 0xF6, 0x00, 0x6B, 0x60, 0x09, 0x9F, 0x69, 0x00, 0x8D, 0xFF, 0xCE, 0xEC, 0xCE, 0x02, 0x59, 0x96, 0x04, 0xEE, 0xA0, 0x83, 0x48, 0x34,
            0xA0, 0x50, 0xA0, 0x5A, 0x5A, 0x5A, 0xEF, 0xBE, 0xF7, 0xFF, 0xFF, 0xFF, 0x00, 0x0F, 0xFF, 0xCC, 0xCC, 0xCC, 0xCC, 0xC3, 0x33, 0xCC, 0xC0, 0x00, 0x33, 0x30, 0x00, 0x00, 0x0C, 0xCC, 0x00, 0x03, 0x33, 0x0F, 0x0F, 0x0F, 0xAA, 0xAA, 0xAA, 0x36, 0xC9, 0x36, 0xC6, 0x39, 0xC6, 0xF4, 0x4F, 0x11,
            0x00, 0x03, 0x22, 0x00, 0x0F, 0x22, 0x00, 0x0E, 0x22, 0x00, 0x77, 0x66, 0x00, 0xFF, 0x66, 0x00, 0xEE, 0x66, 0x00, 0x07, 0x66, 0x00, 0x0F, 0x66, 0x00, 0x0E, 0x66, 0x00, 0x33, 0x22, 0x00, 0xFF, 0x22, 0x00, 0xEE, 0x22, 0x22, 0x22, 0x22, 0x00, 0x0F, 0x00, 0x66, 0x66, 0x66, 0x00, 0xFF, 0x00,
            0x22, 0x23, 0x22, 0x22, 0x2F, 0x22, 0x22, 0x2E, 0x22, 0x66, 0x77, 0x66, 0x66, 0xFF, 0x66, 0x66, 0xEE, 0x66, 0x66, 0x67, 0x66, 0x66, 0x6F, 0x66, 0x66, 0x6E, 0x66, 0x22, 0x33, 0x22, 0x22, 0xFF, 0x22, 0x22, 0xEE, 0x22, 0x00, 0x34, 0x88, 0x00, 0xC2, 0x11, 0x00, 0x13, 0x7F, 0x00, 0x8C, 0xEF,
            0x22, 0x23, 0x00, 0x22, 0x2F, 0x00, 0x22, 0x2E, 0x00, 0x66, 0x77, 0x00, 0x66, 0xFF, 0x00, 0x66, 0xEE, 0x00, 0x66, 0x67, 0x00, 0x66, 0x6F, 0x00, 0x66, 0x6E, 0x00, 0x22, 0x33, 0x00, 0x22, 0xFF, 0x00, 0x22, 0xEE, 0x00, 0x88, 0x43, 0x00, 0x11, 0x2C, 0x00, 0xF7, 0x31, 0x00, 0xFE, 0xC8, 0x00};

        uint8_t pixArray[256 * 256 * 4];
        std::shared_ptr<computer::VRAM> vram;
        uint8_t mode;
        uint8_t drawStates;

        void color2Mode();
        void color16Mode();
        void text256Mode();
        void text128Mode();
        void setPix(uint8_t x, uint8_t y, uint8_t c);
        void setBigPix(uint8_t x, uint8_t y, uint8_t c);
        void setCharPix(uint8_t x, uint8_t y, uint8_t character, uint8_t color);
        void setCharBigPix(uint8_t x, uint8_t y, uint8_t character, uint8_t color);
        void resetDraw();

    public:
        VPU(std::shared_ptr<computer::VRAM> vram);
        ~VPU();

        void setPwr(bool pwr);
        void reset();
        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);

        uint8_t *getPixArray();
    };
} // namespace computer

#endif // VPU_HPP