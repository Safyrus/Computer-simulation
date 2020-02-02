#include <iostream>
#include "Device.hpp"
#include "global.hpp"

Device::Device(int l)
{
    len = l;
    adr = 0;
    data = new int8_t[len];
    for (int i = 0; i < len; i++)
    {
        data[i] = 0;
    }
}

Device::~Device()
{
    if (data != NULL)
    {
        delete data;
    }
}

int Device::getAdr()
{
    return adr;
}

void Device::setAdr(int a)
{
    if (a < len)
    {
        adr = a;
    }
}

int8_t Device::getData()
{
    return data[adr];
}

int Device::getData4()
{
    unsigned int dat = 0;
    dat |= ((((unsigned)data[adr + 0]) & 0xff) << 24);
    if (print_debug)
    std::cout << dat << " ";
    dat |= ((((unsigned)data[adr + 1]) & 0xff) << 16);
    if (print_debug)
    std::cout << dat << " ";
    dat |= ((((unsigned)data[adr + 2]) & 0xff) << 8);
    if (print_debug)
    std::cout << dat << " ";
    dat |= ((((unsigned)data[adr + 3]) & 0xff) << 0);
    if (print_debug)
    std::cout << dat << " \n";
    return dat;
}

void Device::setData(int8_t d)
{
    data[adr] = d;
}

int Device::getLen()
{
    return len;
}