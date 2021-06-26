#include "computer/HardwareStates.hpp"

computer::HardwareStates::HardwareStates()
{
    type = "HWSTATS";
    name = "HWSTATS";
}

computer::HardwareStates::~HardwareStates()
{
}

void computer::HardwareStates::reset()
{
}

void computer::HardwareStates::run()
{
}

void computer::HardwareStates::set(uint16_t adr, uint8_t data)
{
    uint16_t adrTo = adr / 8;
    switch (adrTo)
    {
    case 0:
        break;
    default:
        break;
    }
}

uint8_t computer::HardwareStates::get(uint16_t adr)
{
    uint16_t adrTo = adr / 8;
    uint8_t ret = 0;
    switch (adrTo)
    {
    case 0:
        switch (adr % 8)
        {
        case 0:
            ret = connected;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return ret;
}

void computer::HardwareStates::connect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr)
{
    std::string type = device->getType();
    if (type.compare("ROM") == 0)
    {
        if (endAdr <= 0xFFF)
        {
            connected |= 0x01;
        }
        else if (startAdr >= 0x1000 && endAdr <= 0x17FF)
        {
            connected |= 0x02;
        }
    }
    else if (type.compare("RAM") == 0)
    {
        if (startAdr >= 0x1800 && endAdr <= 0x1BFF)
        {
            connected |= 0x04;
        }
        else if (startAdr >= 0x4000 && endAdr <= 0x77FF)
        {
            connected |= 0x08;
        }
    }
    else if (type.compare("VRAM") == 0)
    {
        if (startAdr >= 0x2000 && endAdr <= 0x3FFF)
        {
            connected |= 0x10;
        }
    }
}

void computer::HardwareStates::disconnect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr)
{
    std::string type = device->getType();
    if (type.compare("ROM") == 0)
    {
        if (endAdr <= 0xFFF)
        {
            connected &= 0xFE;
        }
        else if (startAdr >= 0x1000 && endAdr <= 0x17FF)
        {
            connected &= 0xFD;
        }
    }
    else if (type.compare("RAM") == 0)
    {
        if (startAdr >= 0x1800 && endAdr <= 0x1BFF)
        {
            connected &= 0xFB;
        }
        else if (startAdr >= 0x4000 && endAdr <= 0x77FF)
        {
            connected &= 0xF7;
        }
    }
    else if (type.compare("VRAM") == 0)
    {
        if (startAdr >= 0x2000 && endAdr <= 0x3FFF)
        {
            connected &= 0xEF;
        }
    }
}
