#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include "AssemblerCompiler.hpp"
#include "Computer.hpp"
#include "DISK.hpp"
#include "global.hpp"
#include "console.hpp"
#include "RAM.hpp"
#include "Keyboard.hpp"
#include "Screen.hpp"

bool print_debug;
sf::Font baseFont;

int main()
{
    print_debug = false;
    if (!baseFont.loadFromFile("test.ttf"))
    {
        std::cout << "ERROR: could not load font";
    }

    setupConsole();
    std::cout << "\x1b[1;1H\x1b[2J";

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

    sf::RenderWindow window(sf::VideoMode(640, 360), "S257-01");
    window.setFramerateLimit(1);

    std::cout << "--- What do you want to test ? ---\n0 - Assembler\n1 - CPU\n2 - Input\n3 - Screen\n4 - Computer\n 5 - SFML" << std::endl;
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
        std::cout << "\x1b[1;1H\x1b[2J";
        scr = new Screen();//(0x103);
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
        std::cout << "\x1b[1;1H\x1b[2J";
        com = new Computer();
        disk1 = new DISK(0x8000);
        ram = new RAM(0x1000);
        key = new Keyboard(8);
        scr = new Screen();//(0x103);

        disk1->load("test_com_io");

        com->addDevice(disk1, 0x0000, 0x7FFF);
        com->addDevice(ram, 0x8000, 0x8FFF);
        com->addDevice(key, 0x9000, 0x90FF);
        com->addDevice(scr, 0x9100, 0x9203);

        com->setPwr();
        if (print_debug)
            std::cout << "Computer ON" << std::endl;

        while (com->getPwr())
        {
            com->cycle();
            if (!print_debug)
            {
                com->print(17, 1);
                key->getKey();
                key->print(17, 10);
                if (com->getCycle() % com->getHz() >= com->getHz() / 4)
                {
                    scr->print(1, 1);
                }
            }
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / com->getHz()));
        }
        if (print_debug)
            std::cout << "Computer OFF" << std::endl;

        delete com;
        rawConsole(false);
        break;
    case 5:
        rawConsole(true);
        std::cout << std::hex;
        std::cout << "\x1b[1;1H\x1b[2J";
        com = new Computer();
        disk1 = new DISK(0x8000);
        disk2 = new DISK(0x4000);
        ram = new RAM(0x2000);
        key = new Keyboard(0x100);
        scr = new Screen();

        disk1->load("");
        disk2->load("");

        com->addDevice(disk1, 0x0000, 0x7FFF);
        com->addDevice(disk2, 0x8000, 0xBFFF);
        com->addDevice(ram, 0xC000, 0xDFFF);
        com->addDevice(key, 0xE000, 0xE0FF);
        com->addDevice(scr, 0xE100, 0xE1FF);

        window.setFramerateLimit(60);

        scr->setAdr(3);
        scr->setData(0x80);
        for (int i = 0; i < 16; i+=2)
        {
            scr->setAdr(0x40+i);
            scr->setData(i<<4);
            scr->setAdr(0x40+i+1);
            scr->setData(i+1);
        }
        
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    std::cout << "keypressed \n";
                }
            }

            window.clear(sf::Color(60, 60, 60));

            com->display(window, 0,0);
            scr->display(window, 0xc0, 0);

            window.display();
        }

        delete com;
        rawConsole(false);
        break;
    default:
        break;
    }

    std::cout << "\n-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
    std::cin.ignore();

    std::cout << "\x1b[1;1H\x1b[2J";
    restoreConsole();
}