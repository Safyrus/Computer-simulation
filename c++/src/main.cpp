#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "AssemblerCompiler.hpp"
#include "CPU.hpp"

int main()
{
    std::string file = "";
    int choice = 0;
    CPU *cpu;
    AssemblerCompiler *compiler;
    int adr = 0;

    std::cout << "--- What do you want to test ? ---\n0 - Assembler\n1 - CPU" << std::endl;
    std::cin >> choice;
    switch (choice)
    {
    case 0:
        compiler = new AssemblerCompiler();

        std::cout << "-=#[ Test for assemblerCompiler(loadAssembly) ]#=-" << std::endl;
        std::cout << "--- Enter file name to open ---" << std::endl;
        std::cin >> file;

        compiler->loadAssembler(file.c_str());
        std::cin.ignore();

        std::cout << "\n\n-=#[ Test for assemblerCompiler(saveBinary) ]#=-" << std::endl;
        std::cout << "--- Enter file name to save ---" << std::endl;
        std::cin >> file;

        compiler->saveBinary(file.c_str());

        delete compiler;
        break;
    case 1:

        int disk[0x10];
        for (int i = 0; i < 0x200; i++)
        {
            disk[i] = 0;
        }
        disk[8] = 0x02;
        std::cout << disk[8] << std::endl;
        std::cout << disk[adr] << std::endl;
        std::cout << "array(play role of disk) created" << std::endl;

        cpu = new CPU();
        cpu->setPwr();
        std::cout << "CPU on" << std::endl;

        while (cpu->getPwr())
        {
            adr = cpu->getAdr();
            std::cout << "adr:" << adr << "  disk1:" << disk[adr] << "  disk2:" << disk[adr+1] << "  disk3:" << disk[adr+2] << "  disk4:" << disk[adr+3] << std::endl;
            std::cout << ((disk[adr]&0xff)<<24) << std::endl;
            std::cout << ((disk[adr+1]&0xff)<<16) << std::endl;
            std::cout << ((disk[adr+2]&0xff)<<8) << std::endl;
            std::cout << ((disk[adr+3]&0xff)) << std::endl;
            int data = ((disk[adr]&0xff)<<24) + ((disk[adr+1]&0xff)<<16) + ((disk[adr+2]&0xff)<<8) + (disk[adr+3]&0xff);
            std::cout <<  "data: " << data << std::endl;
            if (!cpu->getClk())
            {
                std::cout << "Clock off" << std::endl;
                cpu->setData(data);
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
                    break;
                case 1:
                    std::cout << "step 1" << std::endl;
                    if (cpu->getLoad())
                    {
                        cpu->setData(data);
                    }
                    else
                    {
                        data = cpu->getData();
                        disk[adr] = data>>24&0xff;
                        disk[adr+1] = data>>16&0xff;
                        disk[adr+2] = data>>8&0xff;
                        disk[adr+3] = data&0xff;
                    }
                    cpu->stp();
                    break;
                case 2:
                    std::cout << "step 2" << std::endl;
                    cpu->setClk();
                    break;
                default:
                    break;
                }
            }
            std::cout << "CPU: adr[" << cpu->getAdr() << "]  data[" << cpu->getData() << "] step[" << cpu->getStep() << "]  clk[" << cpu->getClk() << "]  load[" << cpu->getLoad() << "]  pwr[" << cpu->getPwr() << "]" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        delete cpu;
        break;
    }

    std::cout << "-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
}