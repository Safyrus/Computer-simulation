#include "computer/Bus.hpp"

computer::Bus::Bus()
{
}

computer::Bus::~Bus()
{
}

void computer::Bus::run()
{
}

uint8_t computer::Bus::get(uint16_t adr)
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        if (startAdrs.at(i) <= adr && endAdrs.at(i) >= adr)
        {
            return devices.at(i).get()->get(adr - startAdrs.at(i));
        }
    }
    return 0;
}

void computer::Bus::set(uint16_t adr, uint8_t data)
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        if (startAdrs.at(i) <= adr && endAdrs.at(i) >= adr)
        {
            devices.at(i).get()->set(adr - startAdrs.at(i), data);
        }
    }
}

void computer::Bus::addDevice(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr)
{
    devices.push_back(device);
    startAdrs.push_back(startAdr);
    endAdrs.push_back(endAdr);
}

void computer::Bus::removeDevice(std::shared_ptr<computer::Device> device)
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        if (devices.at(i) == device)
        {
            devices.erase(devices.begin() + i);
            startAdrs.erase(startAdrs.begin() + i);
            endAdrs.erase(endAdrs.begin() + i);
        }
    }
}
