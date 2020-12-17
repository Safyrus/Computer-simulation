#ifndef RUNS257INTERPRETER_HPP
#define RUNS257INTERPRETER_HPP

#include <string>
#include <atomic>

#include "global.hpp"
#include "utils/console.hpp"

#include "computer_old/Computer.hpp"
#include "computer_old/DISK.hpp"
#include "computer_old/RAM.hpp"
#include "computer_old/Keyboard.hpp"
#include "computer_old/Screen.hpp"
#include "computer_old/ScreenSimple.hpp"
#include "computer_old/Timer.hpp"

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

BOOLEAN nanosleepWin(LONGLONG ns)
{
    /* Declarations */
    HANDLE timer;     /* Timer handle */
    LARGE_INTEGER li; /* Time defintion */
    /* Create timer */
    if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;
    /* Set timer properties */
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
    {
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
    Computer *com = (Computer *)lpParameter;
    unsigned int hertz = 10000000;
    if (com != NULL)
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
                nanosleepWin((timePercycle.count() / 10) / hertz);
            }
        }
        else
        {
            nanosleepWin(waiting.count() / 10);
        }
    }
    return 0;
}
#endif

void runS257Interpreter(std::string progPath, int hz, int fps = 60)
{
    // create window
    sf::RenderWindow window(sf::VideoMode(640, 360), "S257-01");
    window.setFramerateLimit(fps);

    // declare thread to run the computer
#ifndef _WIN32
    std::thread comThread;
#else
    HANDLE comThread;
#endif

    // clear console
    rawConsole(true);
    std::cout << std::hex;
    std::cout << "\x1b[1;1H\x1b[2J";

    // create computer components
    Computer *com = new Computer(hz);
    ;
    DISK *disk1 = new DISK(0x8000);
    RAM *ram = new RAM(0x2000);
    Keyboard *key = new Keyboard(0x08);
    ScreenSimple *screen = new ScreenSimple();
    Timer *timer = new Timer();

    // load program into disk
    disk1->load(progPath.c_str());

    // connect components to the cpu
    com->addDevice(disk1, 0x0000, 0x7FFF);
    com->addDevice(ram, 0x8000, 0xDFFC);
    com->addDevice(timer, 0xDFFD, 0xDFFD);
    com->addDevice(key, 0xDFFE, 0xDFFF);
    com->addDevice(screen, 0xE000, 0xFFFF);

    // create computer thread
#ifndef _WIN32
    comThread = std::thread(run, com);
#else
    comThread = CreateThread(
        NULL,  // Thread attributes
        0,     // Stack size (0 = use default)
        run,   // Thread start address
        com,   // Parameter to pass to the thread
        0,     // Creation flags
        NULL); // Thread id
    if (comThread == NULL)
    {
        std::cout << "ERROR\n";
        return;
    }
#endif

    // while the window is open
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

    // tell the computer thread to stop
    stop = true;

    // wait for computer thread to finish
#ifndef _WIN32
    comThread.join();
#else
    WaitForSingleObject(comThread, INFINITE);
    CloseHandle(comThread);
#endif

    // delete computer
    delete com;

    rawConsole(false);
}

#endif // RUNS257INTERPRETER_HPP