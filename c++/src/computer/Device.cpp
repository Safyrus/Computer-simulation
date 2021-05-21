#include "computer/Device.hpp"

computer::Device::Device()
{
    running = false;
    pwr = false;
    type = "NO_VAL";
    name = "NO_NAME";
}

computer::Device::~Device()
{
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

std::string computer::Device::getType()
{
    return type;
}
