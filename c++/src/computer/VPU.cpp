#include "computer/VPU.hpp"

#include <chrono>
#include <math.h>
#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::VPU::VPU(std::shared_ptr<computer::RAM> ram)
{
    type = "VPU";
    this->ram = ram;
    mode = 0;
    reset();
}

computer::VPU::~VPU()
{
}

void computer::VPU::color2Mode()
{
    for (unsigned int i = 0; i < 0x2000; i++)
    {
        uint8_t data = ram->get(i);
        for (uint8_t j = 0; j < 8; j++)
        {
            unsigned int index = (i * 8 * 4) + (j * 4);
            bool pxActive = data & (int)(pow(2, (7 - j)));
            pixArray[index + 0] = pxActive * 255;
            pixArray[index + 1] = pxActive * 255;
            pixArray[index + 2] = pxActive * 255;
            pixArray[index + 3] = 255;
        }
    }
}

void computer::VPU::color16Mode()
{
    for (unsigned int i = 0; i < 0x2000; i++)
    {
        uint8_t data = ram->get(i);
        uint8_t col1 = data >> 4;
        uint8_t col2 = data & 0x0F;
        setBigPix((i * 2) % 128, i / 64, col1);
        setBigPix((i * 2 + 1) % 128, i / 64, col2);
    }
}

void computer::VPU::text256Mode()
{
    for (unsigned int i = 0; i < 0x1500; i += 2)
    {
        uint8_t character = ram->get(i);
        uint8_t color = ram->get(i + 1);
        uint8_t x = (i * 2) % 256;
        uint8_t y = ((i * 2) / 256) * 6;
        setCharPix(x, y, character, color);
    }
    for (unsigned int i = 256*252; i < 256*256; i++)
    {
        pixArray[i*4 + 0] = 0;
        pixArray[i*4 + 1] = 0;
        pixArray[i*4 + 2] = 0;
        pixArray[i*4 + 3] = 255;
    }
}

void computer::VPU::text128Mode()
{
    for (unsigned int i = 0; i < 0x540; i += 2)
    {
        uint8_t character = ram->get(i);
        uint8_t color = ram->get(i + 1);
        uint8_t x = (i * 2) % 128;
        uint8_t y = ((i * 2) / 128) * 6;
        setCharBigPix(x, y, character, color);
    }
    for (unsigned int i = 256*252; i < 256*256; i++)
    {
        pixArray[i*4 + 0] = 0;
        pixArray[i*4 + 1] = 0;
        pixArray[i*4 + 2] = 0;
        pixArray[i*4 + 3] = 255;
    }
}
void computer::VPU::setPix(uint8_t x, uint8_t y, uint8_t c)
{
    c %= 16;
    uint32_t index = (y * 256 + x) * 4;
    pixArray[index + 0] = (PALETTE_16[c] >> 24) & 0xFF;
    pixArray[index + 1] = (PALETTE_16[c] >> 16) & 0xFF;
    pixArray[index + 2] = (PALETTE_16[c] >> 8) & 0xFF;
    pixArray[index + 3] = (PALETTE_16[c] >> 0) & 0xFF;
}

void computer::VPU::setBigPix(uint8_t x, uint8_t y, uint8_t c)
{
    x %= 128;
    y %= 128;
    c %= 16;

    setPix(x * 2, y * 2, c);
    setPix(x * 2 + 1, y * 2, c);
    setPix(x * 2, y * 2 + 1, c);
    setPix(x * 2 + 1, y * 2 + 1, c);
}

void computer::VPU::setCharPix(uint8_t x, uint8_t y, uint8_t character, uint8_t color)
{
    uint32_t charData = (CHAR_MAP[character * 3 + 0]) << 16;
    charData += (CHAR_MAP[character * 3 + 1]) << 8;
    charData += (CHAR_MAP[character * 3 + 2]) << 0;

    uint8_t frontColor = color & 0x0F;
    uint8_t backColor = (color >> 4) & 0x0F;

    for (uint8_t j = 0; j < 6; j++)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            bool px = (charData & 0x800000);

            if (px)
            {
                setPix(x + i, y + j, frontColor);
            }
            else
            {
                setPix(x + i, y + j, backColor);
            }

            charData = charData << 1;
        }
    }
}

void computer::VPU::setCharBigPix(uint8_t x, uint8_t y, uint8_t character, uint8_t color)
{
    uint32_t charData = (CHAR_MAP[character * 3 + 0]) << 16;
    charData += (CHAR_MAP[character * 3 + 1]) << 8;
    charData += (CHAR_MAP[character * 3 + 2]) << 0;

    uint8_t frontColor = color & 0x0F;
    uint8_t backColor = (color >> 4) & 0x0F;

    for (uint8_t j = 0; j < 6; j++)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            bool px = (charData & 0x800000);

            if (px)
            {
                setBigPix(x + i, y + j, frontColor);
            }
            else
            {
                setBigPix(x + i, y + j, backColor);
            }

            charData = charData << 1;
        }
    }
}

void computer::VPU::reset()
{
    for (unsigned int i = 0; i < 256 * 256 * 4; i += 4)
    {
        pixArray[i + 0] = 0;
        pixArray[i + 1] = 0;
        pixArray[i + 2] = 0;
        pixArray[i + 3] = 255;
    }
}

void computer::VPU::run()
{
    while (running)
    {
        if (pwr)
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            unsigned int refreshRate = 60;
            std::chrono::nanoseconds timePercycle(1000000000 / refreshRate);
            switch (mode)
            {
            case 0:
                color2Mode();
                break;
            case 1:
                color16Mode();
                break;
            case 2:
                text256Mode();
                break;
            case 3:
                text128Mode();
                break;
            default:
                reset();
                break;
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            auto ns_int = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
            std::this_thread::sleep_for(timePercycle - ns_int);
        }
        else
        {
            std::chrono::nanoseconds timeWait(1000000);
            std::this_thread::sleep_for(timeWait);
        }
    }
}

void computer::VPU::set(uint16_t adr, uint8_t data)
{
    switch (adr)
    {
    case 0:
        mode = data;
        break;
    default:
        break;
    }
}

uint8_t computer::VPU::get(uint16_t adr)
{
    uint8_t ret = 0;
    switch (adr)
    {
    case 0:
        ret = mode;
        break;
    default:
        break;
    }
    return ret;
}

uint8_t *computer::VPU::getPixArray()
{
    return pixArray;
}