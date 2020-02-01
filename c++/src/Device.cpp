#include <iostream>
#include "Device.hpp"

Device::Device(int l)
{
    len = l;
    adr = 0;
    data = new int8_t[len];
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
    std::cout << dat << " ";
    dat |= ((((unsigned)data[adr + 1]) & 0xff) << 16);
    std::cout << dat << " ";
    dat |= ((((unsigned)data[adr + 2]) & 0xff) << 8);
    std::cout << dat << " ";
    dat |= ((((unsigned)data[adr + 3]) & 0xff) << 0);
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