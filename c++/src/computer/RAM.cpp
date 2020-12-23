#include "computer/RAM.hpp"

#include "utils/hexTxtToBin.hpp"

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::RAM::RAM()
{
    len = 0x0400;
    this->data = new uint8_t[len];
    this->hz = 10000;
}

computer::RAM::RAM(uint16_t size)
{
    len = size;
    this->data = new uint8_t[len];
    this->hz = 10000;
}

computer::RAM::RAM(uint16_t size, uint32_t hz)
{
    len = size;
    this->data = new uint8_t[len];
    this->hz = hz;
}

computer::RAM::~RAM()
{
    delete[] this->data;
}

void computer::RAM::run()
{
    std::chrono::nanoseconds timePercycle(1000000000 / hz);
    while (running)
    {
        for (unsigned int i = 0; i < adrBuf.size(); i++)
        {
            this->data[adrBuf[i]] = dataBuf[i];
        }
        adrBuf.clear();
        dataBuf.clear();
        std::this_thread::sleep_for(timePercycle);
    }
}

void computer::RAM::set(uint16_t adr, uint8_t data)
{
    adrBuf.push_back(adr);
    dataBuf.push_back(data);
}

uint8_t computer::RAM::get(uint16_t adr)
{
    return this->data[adr];
}

void computer::RAM::load(std::string filePath)
{
    std::vector<uint8_t> vals = hexTxtToBin(filePath);
    for (unsigned int i = 0; i < vals.size(); i++)
    {
        this->data[i % len] = vals[i];
    }
}
