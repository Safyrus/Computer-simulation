#include "computer/ROM.hpp"

#include "utils/hexTxtToBin.hpp"
#include "utils/console.hpp"

#include <iostream>

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::ROM::ROM()
{
    type = "ROM";
    name = "ROM";
    len = 0x0400;
    this->data = new uint8_t[len];
    this->hz = 0;
}

computer::ROM::ROM(uint16_t size)
{
    type = "ROM";
    name = "ROM";
    len = size;
    this->data = new uint8_t[len];
    this->hz = 0;
}

computer::ROM::ROM(uint16_t size, uint32_t hz)
{
    type = "ROM";
    name = "ROM";
    len = size;
    this->data = new uint8_t[len];
    this->hz = hz;
}

computer::ROM::~ROM()
{
    delete[] this->data;
}

void computer::ROM::reset()
{
}

void computer::ROM::run()
{
}

void computer::ROM::set(uint16_t adr, uint8_t data)
{
    std::string str = "Can't set data " + data;
    str += " at adr " + adr;
    str += ". Reason: it is a READ ONLY Memory";
    printDebug(str);
}

uint8_t computer::ROM::get(uint16_t adr)
{
    if (!pwr)
    {
        return 0;
    }
    return this->data[adr];
}

void computer::ROM::load(std::string filePath)
{
    std::vector<uint8_t> vals = hexTxtToBin(filePath);
    for (unsigned int i = 0; i < vals.size(); i++)
    {
        this->data[i % len] = vals[i];
    }
}

uint16_t computer::ROM::length()
{
    return len;
}
