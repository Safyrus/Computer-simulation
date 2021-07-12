#include "computer/Keyboard.hpp"
#include "utils/console.hpp"

#include <chrono>
#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::Keyboard::Keyboard()
{
    type = "IOKEY";
    name = "KEYBOARD";
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
    if (talkCPU || bufCurs == 0)
    {
        if (talk)
        {
            talk = false;
            connectedDevice->setTalk(defaultPort, talk);
        }
    }
    else if (connectedDevice)
    {
        if (!talk)
        {
            talk = true;
            connectedDevice->setTalk(defaultPort, talk);
        }
        connectedDevice->send(defaultPort, keyBuf[0]);
        connectedDevice->send(defaultPort, speBuf[0]);
        for (uint8_t i = 0; i < 7; i++)
        {
            keyBuf[i] = keyBuf[i + 1];
            speBuf[i] = speBuf[i + 1];
        }
        bufCurs--;
    }
}

void computer::Keyboard::run()
{
    // declare some variables
    const int SEC_IN_NS = 1000000000;
    const int CYCLE_PER_SEC = 1000000;
    int cnt = 0;
    int hz = 64;
    int nsTime = SEC_IN_NS / hz;
    int cycleTime = CYCLE_PER_SEC / hz;
    bool start = false;
    uint64_t startCycle = cycleCPU;
    int lastCycle = cycleCPU;
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

            // wait
            std::chrono::nanoseconds time(nsTime);
            if (nsTime < minTime)
            {
                time = std::chrono::nanoseconds(minTime);
            }
            std::this_thread::sleep_for(time);

            // execute step(s)
            while (cnt < ((cycleCPU - startCycle) / cycleTime))
            {
                runStep();
                cnt++;
            }
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

void computer::Keyboard::set(uint16_t adr, uint8_t data)
{
}

uint8_t computer::Keyboard::get(uint16_t adr)
{
    return 0;
}

bool computer::Keyboard::getTalk(uint8_t port)
{
    return talk;
}

void computer::Keyboard::setTalk(uint8_t port, bool talk)
{
    talkCPU = talk;
}

void computer::Keyboard::send(uint8_t port, uint8_t data)
{
    if (!talk && talkCPU && cmdCurs < 7)
    {
        cmdBuf[cmdCurs] = data;
        cmdCurs++;
    }
}

void computer::Keyboard::keyPressed(char key, uint8_t special)
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
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (event.key.alt)
        {
            speKey |= 0x04;
        }
        else if (event.key.control)
        {
            speKey |= 0x02;
        }
        else if (event.key.shift)
        {
            speKey |= 0x01;
        }
        break;
    case sf::Event::KeyReleased:
        if (event.key.alt)
        {
            speKey &= 0xFB;
        }
        else if (event.key.control)
        {
            speKey &= 0xFD;
        }
        else if (event.key.shift)
        {
            speKey &= 0xFE;
        }
        break;
    case sf::Event::TextEntered:
        keyPressed((char)(event.text.unicode & 0x7F), speKey);
        break;
    default:
        break;
    }
}