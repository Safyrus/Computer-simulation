#include "computer/Keyboard.hpp"
#include "utils/console.hpp"
#include "utils/sfmlFct.hpp"

#include <chrono>
#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::Keyboard::Keyboard(bool keyLayoutMode)
{
    type = "IOKEY";
    name = "KEYBOARD";
    this->keyLayoutMode = keyLayoutMode;
    reset();
}

computer::Keyboard::~Keyboard()
{
}

void computer::Keyboard::reset()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        keyBuf[i] = 0;
        speBuf[i] = 0;
        cmdBuf[i] = 0;
    }
    bufCurs = 0;
    cmdCurs = 0;
    speKey = 0;
    talk = false;
    talkCPU = false;
    if (connectedDevice)
    {
        talkCPU = connectedDevice->getTalk(defaultPort);
    }
}

void computer::Keyboard::runStep()
{
    if (!connectedDevice)
        return;
    talkCPU = connectedDevice->getTalk(defaultPort);
    if (talkCPU)
    {
        if (talk)
        {
            printDebug("Keyboard: listen to computer");
            talk = false;
            connectedDevice->setTalk(defaultPort, talk);
        }
    }
    else
    {
        if (cmdCurs > 0)
        {
            printDebug("Keyboard: interpreting command...");
            if (cmdBuf[0] == 0x08)
            {
                printDebug("Keyboard: ID command");
                for (uint8_t i = 7; i > 0; i--)
                {
                    keyBuf[i] = keyBuf[i - 1];
                    speBuf[i] = speBuf[i - 1];
                }
                keyBuf[0] = 0x02;
                speBuf[0] = 0x00;
                bufCurs++;
            }
            for (uint8_t i = 0; i < 7; i++)
            {
                cmdBuf[i] = cmdBuf[i + 1];
            }
            cmdCurs--;
            printDebug("Keyboard: done");
        }
        if (bufCurs != 0)
        {
            if (!talk)
            {
                printDebug("Keyboard: talking to computer");
                talk = true;
                connectedDevice->setTalk(defaultPort, talk);
            }
            printDebug("Keyboard: sending data " + std::to_string(keyBuf[0]));
            connectedDevice->send(defaultPort, keyBuf[0]);
            // connectedDevice->send(defaultPort, speBuf[0]);
            for (uint8_t i = 0; i < 7; i++)
            {
                keyBuf[i] = keyBuf[i + 1];
                speBuf[i] = speBuf[i + 1];
            }
            bufCurs--;
        }
        else
        {
            if (talk)
            {
                printDebug("Keyboard: stop talking");
                talk = false;
                connectedDevice->setTalk(defaultPort, talk);
            }
        }
    }
}

void computer::Keyboard::run()
{
    // declare some variables
    const int SEC_IN_NS = 1000000000;
    const int CYCLE_PER_SEC = 1000000;
    uint64_t cnt = 0;
    int hz = 64;
    int nsTime = SEC_IN_NS / hz;
    int cycleTime = CYCLE_PER_SEC / hz;
    bool start = false;
    uint64_t startCycle = cycleCPU;
    uint64_t lastCycle = cycleCPU;
#ifndef _WIN32
    int minTime = 1000;
#else
    int minTime = SEC_IN_NS / 1000;
#endif

    while (running)
    {
        if (pwr)
        {
            // start if not started
            if (!start || cycleCPU < lastCycle)
            {
                start = true;
                cnt = 0;
                startCycle = cycleCPU;
            }
            lastCycle = cycleCPU;

            // execute step(s)
            while (cnt < ((cycleCPU - startCycle) / cycleTime))
            {
                runStep();
                cnt++;
            }

            // wait
            std::chrono::nanoseconds time(nsTime);
            if (nsTime < minTime)
            {
                time = std::chrono::nanoseconds(minTime);
            }
            std::this_thread::sleep_for(time);
        }
        else
        {
            if (start)
            {
                start = false;
            }
            std::chrono::nanoseconds timeWait(SEC_IN_NS / 10);
            std::this_thread::sleep_for(timeWait);
        }
    }
}

void computer::Keyboard::set(uint16_t, uint8_t)
{
}

uint8_t computer::Keyboard::get(uint16_t)
{
    return 0;
}

bool computer::Keyboard::getTalk(uint8_t)
{
    return talk;
}

void computer::Keyboard::setTalk(uint8_t, bool talk)
{
    talkCPU = talk;
}

void computer::Keyboard::send(uint8_t, uint8_t data)
{
    printDebug("Keyboard: receive data " + std::to_string(data));
    talkCPU = connectedDevice->getTalk(defaultPort);
    if (!talk && talkCPU && cmdCurs < 7)
    {
        cmdBuf[cmdCurs] = data;
        cmdCurs++;
    }
    else
    {
        printDebug("Keyboard: can't receive data");
    }
}

void computer::Keyboard::keyPressed(uint8_t key, uint8_t special)
{
    if (bufCurs < 7)
    {
        printDebug("Keyboard: key " + std::to_string(key) + " receive");
        keyBuf[bufCurs] = key;
        speBuf[bufCurs] = special;
        bufCurs++;
    }
}

void computer::Keyboard::inputEvent(sf::Event event)
{
    if (keyLayoutMode)
    {
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // std::string ev = (event.type == sf::Event::KeyPressed) ? "PRESSED" : "RELEASED";
            // std::string str = "KEY " + ev + ":" + std::to_string(event.key.code) + " ctrl:" + std::to_string(event.key.control) + " shft:" + std::to_string(event.key.shift) + " alt:" + std::to_string(event.key.alt);
            // printInfo(str);

            speKey = 0;
            if (event.key.shift)
            {
                speKey |= 0x01;
            }
            if (event.key.control)
            {
                speKey |= 0x02;
            }
            if (event.key.alt)
            {
                speKey |= 0x04;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                speKey |= 0x08;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                uint8_t keyCode = sfmlKeyToSAPHYRKey(event.key.code);
                if (keyCode > 0)
                {
                    uint8_t keyFinal = keyCode;
                    bool bit4 = (keyCode & 0x10);
                    bool bit5 = (keyCode & 0x20);
                    bool bit6 = (keyCode & 0x40);
                    bool shiftNum = (bit4 && !(event.key.shift && (bit4 && bit5)));
                    bool shiftZero = (!shiftNum && keyCode == 0x30);
                    bool shift = (!shiftZero && (event.key.shift || bit5));
                    bool ctrl = (!event.key.control && (bit6 || shiftZero));
                    if (ctrl)
                    {
                        keyFinal |= 0x40;
                    }
                    else
                    {
                        keyFinal &= 0xBF;
                    }
                    if (shift)
                    {
                        keyFinal |= 0x20;
                    }
                    else
                    {
                        keyFinal &= 0xDF;
                    }
                    if (shiftNum || shiftZero)
                    {
                        keyFinal |= 0x10;
                    }
                    else
                    {
                        keyFinal &= 0xEF;
                    }
                    if (shiftZero)
                    {
                        keyFinal |= 0x0F;
                    }
                    if (event.key.alt)
                    {
                        keyFinal += 128;
                    }
                    // printInfo("KEY:" + std::to_string(keyCode) + " CODE:" + std::to_string(keyFinal));
                    keyPressed(keyFinal, speKey);
                }
            }
        }
    }
    else
    {
        uint8_t code = 0;
        switch (event.type)
        {
        case sf::Event::KeyReleased:
        case sf::Event::KeyPressed:
            speKey = 0;
            if (event.key.shift)
            {
                speKey |= 0x01;
            }
            if (event.key.control)
            {
                speKey |= 0x02;
            }
            if (event.key.alt)
            {
                speKey |= 0x04;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                speKey |= 0x08;
            }
            break;
        case sf::Event::TextEntered:
            code = (event.text.unicode & 0xFF);
            keyPressed(code, speKey);
            break;
        default:
            break;
        }
    }
}