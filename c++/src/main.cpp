#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "AssemblerCompiler.hpp"
#include "CPU.hpp"
#include "DISK.hpp"

int main()
{
    std::string file = "";
    int choice = 0;
    AssemblerCompiler *compiler;
    CPU *cpu;
    DISK *disk;

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
        std::cout << std::hex;
        cpu = new CPU();
        disk = new DISK(0x10000);
        std::cout << disk->getLen() << '\n';
        bool load = disk->load("test");
        if (!load)
        {
            std::cout << "ERROR: can't load to disk\n";
        }

        cpu->setPwr();
        std::cout << "CPU on" << std::endl;

        while (cpu->getPwr())
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
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        delete disk;
        delete cpu;
        break;
    }
    std::cout << "-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
}