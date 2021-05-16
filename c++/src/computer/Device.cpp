#include "computer/Device.hpp"

computer::Device::Device()
{
    running = false;
    pwr = false;
    type = DEVICE_TYPE::NOVAL;
    name = "NO_NAME";
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

void computer::Device::setName(std::string name)
{
    this->name = name;
}

std::string computer::Device::getName()
{
    return name;
}
