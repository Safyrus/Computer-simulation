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

#include "global.hpp"
#include "runS257Compiler.hpp"
#include "runS257Interpreter.hpp"

#include "assembler/AssemblerCompiler.hpp"
#include "assembler/Interpreter.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Lexer.hpp"

#include "utils/console.hpp"
#include "utils/hexTxtToBin.hpp"

#include "dynarecs/testDynarecs.hpp"



int main(int argc, char const *argv[])
{
    // define variables
    std::string filePath = "";
    int hz = 8;
    bool test = false;
    int choice = 0;
    AssemblerCompiler *compiler;

    // number of argument verification
    if (argc > 1)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            std::cout << "arg: main [debug][file][hz][test]\n";
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
    if (argc > 4)
    {
        if (strcmp(argv[4], "true") == 0 || strcmp(argv[4], "1") == 0)
            test = true;
    }

    // setup console
    setupConsole();
    clearConsole();

    // load font
    if (!baseFont.loadFromFile("pix46.ttf"))
    {
        printError("Could not load font");
    }

    // wait for input
    std::cout << "--- What do you want to test ? ---\n"
              << ansi(DEFAULT_FG) << "1 - " << ansi(GREEN_FG) << "Assembler\n"
              << ansi(DEFAULT_FG) << "2 - " << ansi(YELLOW_FG) << "Computer(dynarec)(not done)\n"
              << ansi(DEFAULT_FG) << "3 - " << ansi(RED_FG) << "Old assembler(don't use)\n"
              << ansi(DEFAULT_FG) << "4 - " << ansi(GREEN_FG) << "Old computer(interpreter)" << ansi(YELLOW_FG) << "(may be bugged)\n"
              << ansi(RESET);
    std::cin >> choice;

    switch (choice)
    {
    case 1: // sasm Compiler
        runS257Compiler(filePath);
        break;

    case 2: // S257 dynamic recompiler
        if (test)
        {
            testBuffer();
            std::cout << "\n###########################\n";
            testEmitter();
            std::cout << "\n###########################\n";
            testTranslater();
            std::cout << "\n###########################\n";
            testTranslater2(filePath);
            std::cout << "\n###########################\n";
            testDeviceThread(filePath, hz);
            std::cout << "\n###########################\n";
        }
        testGraphicDynarec(filePath, print_debug);
        break;

    case 3: // old sasm compiler
        compiler = new AssemblerCompiler();

        // wait for the user input
        std::cout << "-=#[ Test for assemblerCompiler(loadAssembly) ]#=-" << std::endl;
        std::cout << "--- Enter file name to open ---" << std::endl;
        std::cin >> filePath;

        // compile
        compiler->loadAssembler(filePath.c_str());
        std::cin.ignore();

        // wait for the user input
        std::cout << "\n\n-=#[ Test for assemblerCompiler(saveBinary) ]#=-" << std::endl;
        std::cout << "--- Enter file name to save ---" << std::endl;
        std::cin >> filePath;

        // save
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
    clearConsole();
    restoreConsole();
}
