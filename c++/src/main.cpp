
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
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
#include "console.hpp"

#include "computer_old/Computer.hpp"
#include "computer_old/DISK.hpp"
#include "computer_old/RAM.hpp"
#include "computer_old/Keyboard.hpp"
#include "computer_old/Screen.hpp"
#include "computer_old/ScreenSimple.hpp"
#include "computer_old/Timer.hpp"

#include "dynarecs/testDynarecs.hpp"

#include "utils/hexTxtToBin.hpp"

bool print_debug = false;
sf::Font baseFont;

std::atomic<bool> stop = false;

#ifndef _WIN32
void run(Computer *com)
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    unsigned int hertz = com->getHz() * 2;
    std::chrono::nanoseconds timePercycle(1000000000 / hertz);
    std::chrono::nanoseconds waiting(100000000);
    std::chrono::duration<long, std::nano> time_span;

    while (!stop)
    {
        if (com != NULL && com->getPwr())
        {
            t2 = std::chrono::steady_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::duration<long, std::nano>>(t2 - t1);
            hertz = com->getHz() * 2;
            if (time_span >= timePercycle)
            {
                t1 = std::chrono::steady_clock::now();
                timePercycle = (std::chrono::nanoseconds)(1000000000 / hertz);
                com->halfCycle();
            }
            else
            {
                std::this_thread::sleep_for(timePercycle / hertz);
            }
        }
        else
        {
            std::this_thread::sleep_for(waiting);
        }
    }
}
#else

BOOLEAN nanosleepWin(LONGLONG ns){
	/* Declarations */
	HANDLE timer;	/* Timer handle */
	LARGE_INTEGER li;	/* Time defintion */
	/* Create timer */
	if(!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;
	/* Set timer properties */
	li.QuadPart = -ns;
	if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)){
		CloseHandle(timer);
		return FALSE;
	}
	/* Start & wait for timer */
	WaitForSingleObject(timer, INFINITE);
	/* Clean resources */
	CloseHandle(timer);
	/* Slept without problems */
	return TRUE;
}

DWORD WINAPI run(LPVOID lpParameter)
{
    Computer *com = (Computer*)lpParameter;
    unsigned int hertz = 10000000;
    if(com != NULL)
    {
        hertz = com->getHz() * 2;
    }

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::nanoseconds timePercycle(1000000000 / hertz);
    std::chrono::nanoseconds waiting(100000000);
    std::chrono::duration<long, std::nano> time_span;

    while (!stop)
    {
        if (com != NULL && com->getPwr())
        {
            t2 = std::chrono::steady_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::duration<long, std::nano>>(t2 - t1);
            hertz = com->getHz() * 2;
            if (time_span >= timePercycle)
            {
                for (int i = 0; i < time_span / timePercycle; i++)
                {
                    com->halfCycle();
                }
                t1 = std::chrono::steady_clock::now();
                timePercycle = (std::chrono::nanoseconds)(1000000000 / hertz);
            }
            else
            {
                nanosleepWin((timePercycle.count()/10) / hertz);
            }
        }
        else
        {
            nanosleepWin(waiting.count()/10);
        }
    }
    return 0;
}
#endif


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
    if (argc > 1)
    {
        if (strcmp(argv[1], "true") == 0 || strcmp(argv[1], "1") == 0)
            print_debug = true;
    }
    else
    {
        std::cout << "arg: main [debug][file][hz]\n";
        return 0;
    }
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
    RAM *ram;
    Keyboard *key;
    ScreenSimple *screen;
    Timer *timer;

#ifndef _WIN32
    std::thread comThread;
#else
    HANDLE comThread;
#endif

    //Assembler
    Lexer *lexer;
    Parser *parser;
    Interpreter *interpreter;
    std::vector<Token> tokens;
    std::vector<Node *> nodes;
    bool error = false;
    std::string fileOut = "";
    std::string content = "";

    int fps = 60;
    int hz = 8;
    if (argc > 3)
    {
        hz = std::stoi(argv[3], NULL, 10);
    }

    sf::RenderWindow window(sf::VideoMode(640, 360), "S257-01");
    window.setFramerateLimit(fps);

    std::cout << "--- What do you want to test ? ---\n1 - Assembler\n2 - Computer(dynarec)\n3 - Old assembler(don't use)\n4 - Old computer(interpreter)(don't use)" << std::endl;
    std::cin >> choice;
    switch (choice)
    {
    case 1:
        std::cout << "--- Enter file name to assemble ---" << std::endl;
        std::cin >> file;
        try
        {
            content = openFile(file);
        }
        catch (std::string e)
        {
            std::cout << e;
            break;
        }

        //lexing
        lexer = new Lexer(content, file);
        tokens = lexer->makeToken();
        delete lexer;

        //verif lexing
        for (unsigned int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i].getType() == Token::ERR)
            {
                error = true;
            }
            if (print_debug)
            {
                tokens[i].print();
                std::cout << ", ";
            }
        }
        if (print_debug)
            std::cout << "\n\n";
        if (error)
        {
            std::cout << "Error during Lexing, cannot continue\n"
                      << std::flush;
            break;
        }

        //parsing
        parser = new Parser(tokens);
        nodes = parser->parse();
        delete parser;

        //print parsing
        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            if (print_debug)
                nodes[i]->print();
            if (nodes[i]->getToken(0).getType().compare(Token::ERR) == 0)
            {
                error = true;
            }
            if (print_debug)
                std::cout << ", ";
        }
        if (print_debug)
            std::cout << "\n\n";
        if (error)
        {
            std::cout << "Error during Parsing, cannot continue\n"
                      << std::flush;
            for (unsigned int i = 0; i < nodes.size(); i++)
            {
                delete nodes[i];
            }
            break;
        }

        //interpreting
        interpreter = new Interpreter(nodes, file);
        fileOut = interpreter->interprete();
        delete interpreter;

        //verif interpreting
        std::cout << "\n\n"
                  << std::flush;
        if (print_debug)
            std::cout << fileOut;

        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            delete nodes[i];
        }
        error = fileOut.find("FFFFFFFF") != std::string::npos;
        if (error)
        {
            std::cout << "Error during Interpreting, cannot continue\n";
            break;
        }
        writeFile(fileOut, file.substr(0, file.find_last_of('.')));

        break;
    case 2:
        testBuffer();
        std::cout << "\n###########################\n";
        testEmitter();
        std::cout << "\n###########################\n";
        testTranslater();
        std::cout << "\n###########################\n";
        testTranslater2();
        break;
    case 3:
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
    case 4:
        rawConsole(true);
        std::cout << std::hex;
        std::cout << "\x1b[1;1H\x1b[2J";
        com = new Computer(hz);
        disk1 = new DISK(0x8000);
        ram = new RAM(0x2000);
        key = new Keyboard(0x08);
        screen = new ScreenSimple();
        timer = new Timer();

        if (argc > 2)
        {
            file = argv[2];
        }
        else
        {
            file = "prog/test_com_io";
        }

        disk1->load(file.c_str());

        com->addDevice(disk1, 0x0000, 0x7FFF);
        com->addDevice(ram, 0x8000, 0xDFFC);
        com->addDevice(timer, 0xDFFD, 0xDFFD);
        com->addDevice(key, 0xDFFE, 0xDFFF);
        com->addDevice(screen, 0xE000, 0xFFFF);

        window.setFramerateLimit(fps);

#ifndef _WIN32
        comThread = std::thread(run, com);
#else
        comThread = CreateThread(
            NULL,  // Thread attributes
            0,     // Stack size (0 = use default)
            run,   // Thread start address
            com,  // Parameter to pass to the thread
            0,     // Creation flags
            NULL); // Thread id
        if (comThread == NULL)
        {
            std::cout << "ERROR\n";
            return 1;
        }
#endif

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (event.type == sf::Event::TextEntered)
                {
                    // Handle ASCII characters only
                    if (event.text.unicode < 128)
                    {
                        uint32_t k = event.text.unicode;
                        if ((key->getControl() & 0x02) == 2)
                        {
                            k += 128;
                        }
                        std::cout << "key pressed: " << k << "\n";
                        key->setKey(k);
                    }
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::F1)
                    {
                        com->setPwr();
                    }
                    if (event.key.code == sf::Keyboard::F2)
                    {
                        com->setPause();
                    }
                    if (event.key.code == sf::Keyboard::F3)
                    {
                        hz = (int)(hz / 2.0);
                        if (hz == 0)
                            hz = 1;
                        com->setHz(hz);
                    }
                    if (event.key.code == sf::Keyboard::F4)
                    {
                        hz = (int)(hz * 2.0);
                        com->setHz(hz);
                    }
                    if (event.key.code == sf::Keyboard::LControl)
                    {
                        key->setControl(key->getControl() | 1);
                    }
                    if (event.key.code == sf::Keyboard::LAlt)
                    {
                        key->setControl(key->getControl() | 2);
                    }
                    if (event.key.code == sf::Keyboard::LShift)
                    {
                        key->setControl(key->getControl() | 4);
                    }
                }
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::LControl)
                    {
                        key->setControl(key->getControl() & 0xfe);
                    }
                    if (event.key.code == sf::Keyboard::LAlt)
                    {
                        key->setControl(key->getControl() & 0xfd);
                    }
                    if (event.key.code == sf::Keyboard::LShift)
                    {
                        key->setControl(key->getControl() & 0xfb);
                    }
                }
            }

            window.clear(sf::Color(60, 60, 60));

            int charSizeX = 8;
            int charSizeY = 10;

            screen->display(window, 0, 0);
            com->display(window, 0, charSizeY * 26);
            key->display(window, charSizeX * 32, charSizeY * 34);
            ram->display(window, charSizeX * 32, 0);
            disk1->display(window, charSizeX * 32, charSizeY * 16);

            window.display();
        }
        stop = true;

#ifndef _WIN32
        comThread.join();
#else
        WaitForSingleObject(comThread, INFINITE);
        CloseHandle(comThread);
#endif

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
