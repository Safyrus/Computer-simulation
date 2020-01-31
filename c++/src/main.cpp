#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "AssemblerCompiler.hpp"
#include "Computer.hpp"

int main()
{
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
        com = new Computer("test");
        com->setPwr();
        std::cout << "Computer ON" << std::endl;

        while (com->getPwr())
        {
            com->cycle();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        delete com;
        break;
    }
    std::cout << "-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}