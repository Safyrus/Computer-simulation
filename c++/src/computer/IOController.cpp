#include "computer/IOController.hpp"
#include "utils/console.hpp"
#include <chrono>

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

computer::IOController::IOController()
{
    type = "IOCTRL";
    name = "IOCTRL";
    reset();
}

computer::IOController::~IOController()
{
    for (uint8_t i = 0; i < DEVICES_SIZE; i++)
    {
        if (runnables[i])
        {
            runnables[i]->stop();
            runnables[i]->join();
            delete runnables[i];
        }
    }
}

void computer::IOController::reset()
{
    talkCPU = 0;
    talkDEV = 0;
    connected = 0;
    for (uint8_t i = 0; i < DEVICES_SIZE; i++)
    {
        if (devices[i])
        {
            connected |= (0x01 << i);
            talkDEV |= (devices[i]->getTalk(0) << i);
            devices[i]->reset();
        }
        inBufState[i] = 0;
        outBufState[i] = 0;
        for (uint8_t j = 0; j < OUT_SIZE; j++)
        {
            outBuffer[i][j] = 0;
        }
        for (uint8_t j = 0; j < IN_SIZE; j++)
        {
            inBuffer[i][j] = 0;
        }
    }
}

void computer::IOController::setPwr(bool pwr)
{
    this->pwr = pwr;
    for (uint8_t i = 0; i < DEVICES_SIZE; i++)
    {
        if (devices[i])
        {
            devices[i]->setPwr(pwr);
        }
    }
}

void computer::IOController::refreshCycle(uint64_t cycle)
{
    cycleCPU = cycle;
    for (uint8_t i = 0; i < DEVICES_SIZE; i++)
    {
        if(devices[i])
        {
            devices[i]->refreshCycle(cycleCPU);
        }
    }
}


void computer::IOController::runStep()
{
    for (uint8_t i = 0; i < DEVICES_SIZE; i++)
    {
        if (outBufState[i] != 0 && getTalk(i) && !getTalkDev(i))
        {
            devices[i]->send(0, outBuffer[i][0]);
            for (uint8_t j = 0; j < OUT_SIZE - 1; j++)
            {
                outBuffer[i][j] = outBuffer[i][j + 1];
            }
            outBuffer[i][OUT_SIZE - 1] = 0;
            outBufState[i]--;
        }
    }
}

void computer::IOController::run()
{
    // declare some variables
    const int SEC_IN_NS = 1000000000;
    const int CYCLE_PER_SEC = 1000000;
    uint64_t cnt = 0;
    int hz = 1024;
    int nsTime = SEC_IN_NS / hz;
    int cycleTime = CYCLE_PER_SEC / hz;
    bool start = false;
    uint64_t lastCycle = cycleCPU;
    uint64_t startCycle = cycleCPU;
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

void computer::IOController::set(uint16_t adr, uint8_t data)
{
    if (adr < 32)
    {
        uint8_t port = adr / 4;
        uint8_t reg = adr % 4;
        switch (reg)
        {
        case 1: // set
            if (outBufState[port] < OUT_SIZE)
            {
                outBuffer[port][outBufState[port]] = data;
                outBufState[port]++;
            }
            else
            {
                printDebug("IOController(set)(port " + std::to_string(port) + "): Buffer full");
            }
            break;
        case 0: // get
        case 2: // stat get
        case 3: // stat set
            printError("IOController(set)(port " + std::to_string(port) + "): can't set a read only register");
            break;
        }
    }
    else
    {
        switch (adr - 32)
        {
        case 0:
        case 2:
            printError("IOController(set): can't set a read only register");
            break;
        case 1:
            talkCPU = data;
            break;
        default:
            printError("IOController(set): nothing at address " + adr);
            break;
        }
    }
}

uint8_t computer::IOController::get(uint16_t adr)
{
    uint8_t ret = 0;
    if (adr < 32)
    {
        uint8_t port = adr / 4;
        uint8_t reg = adr % 4;
        switch (reg)
        {
        case 0: // get
            ret = inBuffer[port][0];
            for (uint8_t i = 0; i < IN_SIZE - 1; i++)
            {
                inBuffer[port][i] = inBuffer[port][i + 1];
            }
            inBuffer[port][IN_SIZE - 1] = 0;
            if (inBufState[port] > 0)
            {
                inBufState[port]--;
            }
            break;
        case 1: // set
            printError("IOController(get): can't get a write only register");
            break;
        case 2: // stat get
            ret = inBufState[port];
            break;
        case 3: // stat set
            ret = outBufState[port];
            break;
        }
    }
    else
    {
        switch (adr - 32)
        {
        case 0:
            ret = connected;
            break;
        case 1:
            ret = talkCPU;
            break;
        case 2:
            ret = talkDEV;
            break;
        default:
            printError("IOController(get): nothing at address " + adr);
            break;
        }
    }
    return ret;
}

void computer::IOController::addIO(std::shared_ptr<computer::IODevice> device, uint8_t port)
{
    if (port >= DEVICES_SIZE)
    {
        printDebug("IOController(addIO): Wrong port number");
        return;
    }
    if (devices[port] != nullptr)
    {
        printDebug("IOController(addIO): Port already used");
        return;
    }
    devices[port] = device;
    runnables[port] = new RunnableDevice(devices[port]);
    runnables[port]->run();
    connected |= (0x01 << port);
    devices[port]->setPwr(pwr);
    device->setDefaultPort(port);
    device->setConnectedDevice(shared_from_this());
}

void computer::IOController::removeIO(uint8_t port)
{
    if (port >= DEVICES_SIZE)
    {
        printDebug("IOController(removeIO): Wrong port number");
        return;
    }
    runnables[port]->stop();
    runnables[port]->join();
    delete runnables[port];
    devices[port] = nullptr;
    connected &= (0xFF ^ (0x01 << port));
}

bool computer::IOController::getTalk(uint8_t port)
{
    return talkCPU & (0x01 << port);
}

bool computer::IOController::getTalkDev(uint8_t port)
{
    return talkDEV & (0x01 << port);
}

void computer::IOController::setTalk(uint8_t port, bool talk)
{
    if (talk)
    {
        talkDEV |= (talk << port);
    }
    else
    {
        talkDEV &= (0xFF ^ (0x01 << port));
    }
}

void computer::IOController::setTalkCPU(uint8_t port, bool talk)
{
    if (talk)
    {
        talkCPU |= (talk << port);
    }
    else
    {
        talkCPU &= (0xFF ^ (0x01 << port));
    }
}

void computer::IOController::send(uint8_t port, uint8_t data)
{
    if (port >= DEVICES_SIZE)
    {
        printError("IOController(send): Wrong port number");
        return;
    }
    if (getTalk(port))
    {
        printDebug("IOController(send): can't receive data when CPU talk");
        return;
    }
    if (!getTalkDev(port))
    {
        printDebug("IOController(send): can't receive data when DEV not talking");
        return;
    }
    printDebug("IOController(send): receive data " + std::to_string(data) + " from port " + std::to_string(port));
    if (inBufState[port] < IN_SIZE - 1)
    {
        inBuffer[port][inBufState[port]] = data;
        inBufState[port]++;
    }
    else
    {
        printDebug("IOController(send): Buffer full");
    }
}

uint8_t computer::IOController::getOutBuf(uint8_t port, uint8_t index)
{
    if (port < DEVICES_SIZE && index < OUT_SIZE)
        return outBuffer[port][index];
    return 0;
}

uint8_t computer::IOController::getInBuf(uint8_t port, uint8_t index)
{
    if (port < DEVICES_SIZE && index < IN_SIZE)
        return inBuffer[port][index];
    return 0;
}
