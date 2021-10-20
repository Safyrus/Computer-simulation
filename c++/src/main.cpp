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

#include "assembler/Interpreter.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Lexer.hpp"

#include "utils/console.hpp"
#include "utils/hexTxtToBin.hpp"

#include "dynarecs/testDynarecs.hpp"

#include "graphic/MainWindow.hpp"

#include "linker/Linker.hpp"

void runMainWindow(bool print_debug, std::string filePath, uint32_t hz)
{
    printDebug("Create MainWindow");
    std::shared_ptr<graphic::MainWindow> app = std::make_shared<graphic::MainWindow>("S257 Dynamic Recompiler - Main Window", print_debug, filePath, hz);
    //std::cout << "Load ComWinManager config" << std::endl;
    //app->loadConfig(filePath);
    printDebug("Run MainWindow");
    app->display();
    printDebug("End MainWindow");
}

int main(int argc, char const *argv[])
{
    // define variables
    std::string filePath = "";
    std::string str;
    std::string linkFile;
    std::string labelFile;
    std::string outFile;
    uint32_t hz = 8;
    bool test = false;
    bool error = false;
    int choice = 0;
    Linker linker;

    // number of argument verification
    if (argc > 1)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            std::cout << "arg: main [debug][file][hz][test]\n";
            std::cout << "     link [debug][linkFile][labelFile][outFile]\n";
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
        try
        {
            hz = std::stoi(argv[3], NULL, 10);
        }
        catch(const std::exception& e)
        {
            hz = 0;
        }
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
              << ansi(DEFAULT_FG) << "2 - " << ansi(GREEN_FG) << "Computer(dynarec)\n"
              << ansi(DEFAULT_FG) << "3 - " << ansi(GREEN_FG) << "Linker\n"
              << ansi(RESET);
    std::cin >> choice;

    switch (choice)
    {
    case 1: // sasm Compiler
        error = !runS257Compiler(filePath);
        break;

    case 2: // S257 dynamic recompiler
        if (test)
        {
            testBuffer();
            std::cout << "\n###########################\n";
            testEmitter();
            std::cout << "\n###########################\n";
            //testTranslater();
            //std::cout << "\n###########################\n";
            //testTranslater2(filePath);
            //std::cout << "\n###########################\n";
            testDeviceThread(filePath, hz);
            std::cout << "\n###########################\n";
        }

        runMainWindow(print_debug, filePath, hz);
        break;
    case 3: // linker
        linkFile = filePath;
        if (argc > 3)
        {
            labelFile = argv[3];
        }
        if (argc > 4)
        {
            outFile = argv[4];
        }
        error = linker.link(linkFile, labelFile, outFile);
        break;

    default:
        break;
    }

    // wait for user to close the console
    std::cout << "\n-=#[ Done ]#=-" << std::endl;

    // restore console
    if (!print_debug && !error)
        clearConsole();
    restoreConsole();
    return error;
}
