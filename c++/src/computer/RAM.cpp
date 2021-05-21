#include "computer/RAM.hpp"

#include "utils/hexTxtToBin.hpp"

#include <iostream>

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::RAM::RAM()
{
    type = "RAM";
    len = 0x0400;
    this->data = new uint8_t[len];
    this->hz = 0;
}

computer::RAM::RAM(uint16_t size)
{
    type = "RAM";
    len = size;
    this->data = new uint8_t[len];
    this->hz = 0;
}

computer::RAM::RAM(uint16_t size, uint32_t hz)
{
    type = "RAM";
    len = size;
    this->data = new uint8_t[len];
    this->hz = hz;
}

computer::RAM::~RAM()
{
    delete[] this->data;
}

void computer::RAM::reset()
{
    delete[] this->data;
    this->data = new uint8_t[len];
    for (uint16_t i = 0; i < len; i++)
    {
        data[i] = 0;
    }
    
}


void computer::RAM::run()
{
    while (running)
    {
        if (hz > 0 && pwr)
        {
            std::chrono::nanoseconds timePercycle(1000000000 / hz);
            for (unsigned int i = 0; i < adrBuf.size(); i++)
            {
                this->data[adrBuf[i]] = dataBuf[i];
            }
            adrBuf.clear();
            dataBuf.clear();
            std::this_thread::sleep_for(timePercycle);
        }else
        {
            std::chrono::nanoseconds timeWait(1000000);
            std::this_thread::sleep_for(timeWait);
        }
    }
}

void computer::RAM::set(uint16_t adr, uint8_t data)
{
    if(!pwr)
        return;
    if (hz == 0)
    {
        this->data[adr] = data;
    }else
    {
        adrBuf.push_back(adr);
        dataBuf.push_back(data);
    }
}

uint8_t computer::RAM::get(uint16_t adr)
{
    if(!pwr)
    {
        return 0;
    }
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

uint16_t computer::RAM::length()
{
    return len;
}
