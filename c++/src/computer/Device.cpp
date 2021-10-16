#include "computer/Device.hpp"
#include "utils/console.hpp"

computer::Device::Device()
{
    running = false;
    pwr = false;
    type = "NO_VAL";
    name = "NO_NAME";
    cycleCPU = 0;
    printDebug("Device Creation");
}

computer::Device::~Device()
{
    printDebug(name + ": Device Destruction");
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

void computer::Device::refreshCycle(uint64_t cycle)
{
    cycleCPU = cycle;
}
