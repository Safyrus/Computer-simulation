
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string.h>

#ifndef _WIN32
#include <thread>
#else
#include <windows.h>
#include <time.h>
#endif

#include <SFML/Graphics.hpp>

#include "assembler/AssemblerCompiler.hpp"
#include "assembler/Interpreter.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Lexer.hpp"

#include "global.hpp"
#include "utils/console.hpp"
#include "runS257Compiler.hpp"
#include "runS257Interpreter.hpp"

#include "dynarecs/testDynarecs.hpp"

#include "utils/hexTxtToBin.hpp"

bool print_debug = false;
sf::Font baseFont;

std::string openFile(std::string f)
{
    //open file
    std::string fileName = f;
    std::ifstream file;
    file.open(f);
    if (!file.is_open())
    {
        std::string e = "Unable to open file " + fileName + "\n";
        throw e;
    }
    else
    {
        if (print_debug)
            std::cout << "file " + fileName + " open \n";
    }

    // get file content
    if (print_debug)
        std::cout << "read file " + fileName + " ...";
    std::string str = "";
    std::string line = "";
    while (getline(file, line))
    {
        str += line + '\n';
    }
    if (print_debug)
        std::cout << "done\n";

    //close file
    file.close();
    if (print_debug)
        std::cout << "file " + fileName + " close\n";

    return str;
}

void writeFile(std::string content, std::string fileName)
{
    std::ofstream file;
    file.open(fileName);
    int spaceCount = 0;
    for (unsigned int i = 0; i < content.size(); i++)
    {
        file << content[i];
        if (content[i] == ' ')
        {
            spaceCount++;
            if (spaceCount >= 16)
            {
                file << '\n';
                spaceCount = 0;
            }
            else if (spaceCount % 4 == 0)
            {
                file << ' ';
            }
        }
    }
    file.close();
}

int main(int argc, char const *argv[])
{
    // define variables
    std::string filePath = "";
    int hz = 8;
    int choice = 0;
    AssemblerCompiler *compiler;

    // number of argument verification
    if (argc > 1)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            std::cout << "arg: main [debug][file][hz]\n";
            return 0;
        }
        if (strcmp(argv[1], "true") == 0 || strcmp(argv[1], "1") == 0)
            print_debug = true;
    }
    // filePath
    if (argc > 2)
    {
        filePath = argv[2];
    }
    // hz
    if (argc > 3)
    {
        hz = std::stoi(argv[3], NULL, 10);
    }
    // load font
    if (!baseFont.loadFromFile("pix46.ttf"))
    {
        std::cout << "ERROR: could not load font";
    }

    // setup console
    setupConsole();
    std::cout << "\x1b[1;1H\x1b[2J";

    // wait for input
    std::cout << "--- What do you want to test ? ---\n1 - Assembler\n2 - Computer(dynarec)\n3 - Old assembler(don't use)\n4 - Old computer(interpreter)(don't use)" << std::endl;
    std::cin >> choice;

    switch (choice)
    {
    case 1: // sasm Compiler
        runS257Compiler(filePath);
        break;
    case 2: // S257 dynamic recompiler
        testBuffer();
        std::cout << "\n###########################\n";
        testEmitter();
        std::cout << "\n###########################\n";
        testTranslater();
        std::cout << "\n###########################\n";
        testTranslater2();
        break;
    case 3: // old sasm compiler
        compiler = new AssemblerCompiler();

        std::cout << "-=#[ Test for assemblerCompiler(loadAssembly) ]#=-" << std::endl;
        std::cout << "--- Enter file name to open ---" << std::endl;
        std::cin >> filePath;

        compiler->loadAssembler(filePath.c_str());
        std::cin.ignore();

        std::cout << "\n\n-=#[ Test for assemblerCompiler(saveBinary) ]#=-" << std::endl;
        std::cout << "--- Enter file name to save ---" << std::endl;
        std::cin >> filePath;

        compiler->saveBinary(filePath.c_str());
        delete compiler;
        break;
    case 4: // S257 interpreter
        runS257Interpreter(filePath, hz);
        break;
    default:
        break;
    }

    // wait for user to close the console
    std::cout << "\n-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
    std::cin.ignore();

    // restore console
    std::cout << "\x1b[1;1H\x1b[2J";
    restoreConsole();
}
