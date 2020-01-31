#include <iostream>
#include "Computer.hpp"

Computer::Computer(const char *f)
{
    cpu = new CPU();
    disk = new DISK(0x10000);
    bool load = disk->load(f);
    if (!load)
    {
        std::cout << "ERROR: can't load to disk\n";
    }
}

Computer::~Computer()
{
    delete cpu;
    delete disk;
}

void Computer::cycle()
{
    if (cpu->getAdr() < disk->getLen())
    {
        disk->setAdr(cpu->getAdr());
        int data4 = disk->getData4();
        std::cout << "data: " << data4 << std::endl;
        if (!cpu->getClk())
        {
            std::cout << "Clock off" << std::endl;
            cpu->setData(data4);
            cpu->setClk();
            cpu->stp();
        }
        else
        {
            std::cout << "Clock on" << std::endl;
            switch (cpu->getStep())
            {
            case 0:
                std::cout << "step 0" << std::endl;
                cpu->setClk();
                cpu->stp();
                break;
            case 1:
                std::cout << "step 1" << std::endl;
                if (cpu->getLoad())
                {
                    std::cout << "DATA load: " << disk->getData() << std::endl;
                    cpu->setData(disk->getData());
                }
                else
                {
                    int data = cpu->getData();
                    std::cout << "DATA save: " << data << std::endl;
                    disk->setData((data & 0xff));
                }
                cpu->stp();
                break;
            case 2:
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