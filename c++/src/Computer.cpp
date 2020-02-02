#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "Computer.hpp"
#include "global.hpp"

Computer::Computer()
{
    cpu = new CPU();
}

Computer::~Computer()
{
    for (int i = 0; i < devices.size(); i++)
    {
        delete devices[i];
    }
    delete cpu;
}

void Computer::cycle()
{
    int dev = -1;
    for (int i = 0; i < devices.size(); i++)
    {
        if (cpu->getAdr() >= adrDeviceStart[i] && cpu->getAdr() <= adrDeviceEnd[i])
        {
            dev = i;
            break;
        }
    }

    if (dev != -1)
    {
        devices[dev]->setAdr(cpu->getAdr() - adrDeviceStart[dev]);
        int data4 = devices[dev]->getData4();
        if (print_debug)
            std::cout << "data: " << data4 << std::endl;
        if (!cpu->getClk())
        {
            if (print_debug)
                std::cout << "Clock off" << std::endl;
            cpu->setData(data4);
            cpu->setClk();
            cpu->stp();
        }
        else
        {
            if (print_debug)
                std::cout << "Clock on" << std::endl;
            switch (cpu->getStep())
            {
            case 0:
                if (print_debug)
                    std::cout << "step 0" << std::endl;
                cpu->setClk();
                cpu->stp();
                break;
            case 1:
                if (print_debug)
                    std::cout << "step 1" << std::endl;
                if (cpu->getLoad())
                {
                    if (print_debug)
                        std::cout << "DATA load: " << devices[dev]->getData() << std::endl;
                    cpu->setData(devices[dev]->getData());
                }
                else
                {
                    int data = cpu->getData();
                    if (print_debug)
                        std::cout << "DATA save: " << data << std::endl;
                    devices[dev]->setData((data & 0xff));
                }
                cpu->stp();
                break;
            case 2:
                if (print_debug)
                    std::cout << "step 2" << std::endl;
                cpu->setClk();
                cpu->stp();
                break;
            default:
                break;
            }
        }
    }
    else
    {
        cpu->setClk();
    }

    if (print_debug)
        std::cout << "CPU: adr[" << cpu->getAdr() << "]  data[" << cpu->getData() << "] step[" << cpu->getStep() << "]  clk[" << cpu->getClk() << "]  load[" << cpu->getLoad() << "]  pwr[" << cpu->getPwr() << "]" << std::endl;
}

void Computer::setPwr()
{
    cpu->setPwr();
}

bool Computer::getPwr()
{
    return cpu->getPwr();
}

void Computer::addDevice(Device *d, int adrStart, int adrEnd)
{
    devices.push_back(d);
    adrDeviceStart.push_back(adrStart);
    adrDeviceEnd.push_back(adrEnd);
}

void Computer::removeDevice(Device *d)
{
    std::vector<Device *>::iterator itDev;
    std::vector<int>::iterator itStart;
    std::vector<int>::iterator itEnd;
    itStart = adrDeviceStart.begin();
    itEnd = adrDeviceEnd.begin();

    for (itDev = devices.begin(); itDev != devices.end(); itDev++)
    {
        if (d == *itDev)
        {
            devices.erase(itDev);
            adrDeviceStart.erase(itStart);
            adrDeviceEnd.erase(itEnd);
            break;
        }
        itStart++;
        itEnd++;
    }
}

void Computer::print(int x, int y)
{
    std::stringstream ss;
    ss << "\x1b[" << y << ";" << x << "H";
    std::cout << ss.str() << "|CPU:  stp=" << cpu->getStep() << "  clk=" << cpu->getClk() << "  pwr=" << cpu->getPwr() << "  load=" << cpu->getLoad();
    ss.clear();
    ss << "\x1b[" << y+1 << ";" << x << "H";
    std::cout << ss.str() << "|ADR:  " << std::setfill('0') << std::setw(4) << cpu->getAdr();
    ss.clear();
    ss << "\x1b[" << y+2 << ";" << x << "H";
    std::cout << ss.str() << "|DATA: ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData()>>24)&0xff) << "  ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData()>>16)&0xff) << "  ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData()>>8)&0xff) << "  ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData())&0xff);
    std::cout << std::flush;
    cpu->print(x, y+3);
}