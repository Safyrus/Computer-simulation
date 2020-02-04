#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "AssemblerCompiler.hpp"
#include "Computer.hpp"
#include "DISK.hpp"
#include "global.hpp"
#include "console.hpp"
#include "RAM.hpp"
#include "Keyboard.hpp"
#include "Screen.hpp"

bool print_debug;

int main()
{
    print_debug = false;
    setupConsole();

    std::string file = "";
    int choice = 0;
    AssemblerCompiler *compiler;
    Computer *com;
    DISK *disk1;
    DISK *disk2;
    RAM *ram;
    Keyboard *key;
    Screen *scr;
    int8_t c;


    std::cout << "--- What do you want to test ? ---\n0 - Assembler\n1 - CPU\n2 - Input\n3 - Screen\n4 - Computer" << std::endl;
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
        disk1 = new DISK(0x100);
        disk2 = new DISK(0x7F00);
        disk1->load("test1");
        disk2->load("test2");
        com->addDevice(disk1, 0, 0xFF);
        com->addDevice(disk2, 0x100, 0x7FFF);

        com->setPwr();
        if (print_debug)
            std::cout << "Computer ON" << std::endl;

        while (com->getPwr())
        {
            com->cycle();
            if (!print_debug)
                com->print(10, 5);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (print_debug)
            std::cout << "Computer OFF" << std::endl;
        com->removeDevice(disk1);
        com->removeDevice(disk2);

        delete com;
        delete disk1;
        delete disk2;
        break;
    case 2:
        rawConsole(true);
        key = new Keyboard(8);
        c = 0;
        while (c != 27)
        {
            key->getKey();
            c = key->getData();
            std::cout << (char)c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        delete key;
        rawConsole(false);
        break;
    case 3:
        scr = new Screen(0x103);
        scr->setAdr(0);
        scr->setData(16);
        scr->setAdr(1);
        scr->setData(8);
        scr->setAdr(2);
        scr->setData(0);
        for (int i = 3; i < scr->getLen(); i++)
        {
            scr->setAdr(i);
            scr->setData(rand() % 2);
        }
        scr->print(0, 0);

        delete scr;
        break;
    case 4:
        rawConsole(true);
        std::cout << std::hex;
        std::cout << "\x1b[2J";
        com = new Computer();
        disk1 = new DISK(0x8000);
        ram = new RAM(0x1000);
        key = new Keyboard(8);
        scr = new Screen(0x103);

        disk1->load("test_com_io");

        com->addDevice(disk1, 0x0000, 0x7FFF);
        com->addDevice(ram, 0x8000, 0x8FFF);
        com->addDevice(key, 0x9000, 0x9007);
        com->addDevice(scr, 0x9100, 0x9203);

        com->setPwr();
        if (print_debug)
            std::cout << "Computer ON" << std::endl;

        int cycle = 0;
        int Hz = 50;
        int refresh = 0;
        while (com->getPwr())
        {
            com->cycle();
            if (!print_debug)
            {
                com->print(17, 1);
                key->getKey();
                key->print(17, 10);
                if (cycle % Hz >= Hz / 4)
                {
                    cycle = 0;
                    scr->print(1, 1);
                    refresh++;
                    std::cout << refresh;
                }
            }
            cycle++;
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / Hz));
        }
        if (print_debug)
            std::cout << "Computer OFF" << std::endl;

        delete com;
        delete disk1;
        delete ram;
        delete key;
        delete scr;
        rawConsole(false);
    }

    std::cout << "\n-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
    std::cin.ignore();

    restoreConsole();
}