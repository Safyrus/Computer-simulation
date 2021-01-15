#include "computer/Device.hpp"

computer::Device::Device()
{
    running = false;
    pwr = false;
    type = DEVICE_TYPE::NOVAL;
}

computer::Device::~Device()
{
}

DEVICE_TYPE computer::Device::getType()
{
    return type;
}

void computer::Device::setPwr(bool pwr)
{
    this->pwr = pwr;
}

bool computer::Device::getPwr()
{
    return pwr;
}
