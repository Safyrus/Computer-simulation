#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "AssemblerCompiler.hpp"
#include "Computer.hpp"
#include "DISK.hpp"
#include "global.hpp"
#include "console.hpp"

bool print_debug;

int main()
{
    print_debug = false;
    setupConsole();

    std::string file = "";
    int choice = 0;
    AssemblerCompiler *compiler;
    Computer *com;

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

        com = new Computer();
        DISK *disk1 = new DISK(0x100);
        DISK *disk2 = new DISK(0xFF00);
        disk1->load("test1");
        disk2->load("test2");
        com->addDevice(disk1, 0, 0xFF);
        com->addDevice(disk2, 0x100, 0xFFFF);

        com->setPwr();
        if (print_debug)
            std::cout << "Computer ON" << std::endl;

        while (com->getPwr())
        {
            com->cycle();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (print_debug)
            std::cout << "Computer OFF" << std::endl;
        com->removeDevice(disk1);
        com->removeDevice(disk2);

        delete com;
        delete disk1;
        delete disk2;
        break;
    }

    std::cout << "-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
    std::cin.ignore();

    restoreConsole();
}