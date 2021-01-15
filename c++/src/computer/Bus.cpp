#include "computer/Bus.hpp"

#include <iostream>

computer::Bus::Bus()
{
    type = DEVICE_TYPE::BUS;
}

computer::Bus::~Bus()
{
}

void computer::Bus::run()
{
}

void computer::Bus::reset()
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        devices[i]->reset();
    }
}

void computer::Bus::setPwr(bool pwr)
{
    this->pwr = pwr;
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        devices[i]->setPwr(pwr);
        std::cout << "pwr["+std::to_string(i)+"]=" << devices[i]->getPwr() << "\n";
    }
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
    std::cout << "no device found\n";
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

uint32_t computer::Bus::getDeviceAdr(std::shared_ptr<computer::Device> device)
{
    uint32_t res = 0;
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        if (devices.at(i) == device)
        {
            res = (startAdrs[i] << 16);
            res += endAdrs[i];
            return res;
        }
    }
    return res;
}
