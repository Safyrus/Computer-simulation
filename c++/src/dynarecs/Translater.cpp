#include "dynarecs/Translater.hpp"
#include "computer/instruction.h"
#include "utils/console.hpp"
#ifndef _WIN32
#include "dynarecs/Emitter64.hpp"
#else
#include "dynarecs/Emitter86.hpp"
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

dynarec::Translater::Translater(std::shared_ptr<computer::CPU> cpu, bool rawBus)
{
    this->cpu = cpu;
    this->rawBus = rawBus;
    running = false;
    pause = false;
    for (int i = 0; i < 1024 * 64; i++)
    {
        blocks.push_back(nullptr);
    }
    blockSize = MAX_BLOCK_SIZE;
}

dynarec::Translater::~Translater()
{
    deleteBlocks();
}

void dynarec::Translater::deleteBlocks()
{
    for (unsigned int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i] != nullptr)
        {
            if (print)
            {
                printDebug(ansi(WHITE_FG) + "delete block " + std::to_string(i));
            }
            delete blocks[i];
            blocks[i] = nullptr;
        }
    }
}

void dynarec::Translater::deleteBlocks(uint16_t adr)
{
    for (uint16_t i = 0; i < blockSize; i++)
    {
        uint16_t index = adr - i;
        if (blocks[index] != nullptr)
        {
            printDebug(ansi(WHITE_FG) + "delete block " + std::to_string(index));
            delete blocks[index];
            blocks[index] = nullptr;
        }
    }
}

dynarec::Emitter *dynarec::Translater::handlerEndBlock(int ret)
{
    // print debug msg
    if (print)
        printDebug(ansi(YELLOW_FG) + "| handler end block" + ansi(RESET));

    // decode value return from the block
    uint8_t val = ((ret & 0xff000000) >> 24);
    uint16_t adrJmp = ((ret & 0xffff00) >> 8);
    uint8_t code = (ret & 0xff);

    // find what to do
    switch (code)
    {
    case CODE_RET: // end of the program
        if (print)
            printDebug(ansi(YELLOW_FG) + "|     CODE_RET: stopping cpu" + ansi(RESET));
        running = false;
        cpu->setPwr(false);
        break;

    case CODE_RST: // reset the cpu
        if (print)
            printDebug(ansi(YELLOW_FG) + "|     CODE_RST: resetting cpu" + ansi(RESET));
        cpu->reset();
        return getBlock(cpu->pc);
        break;

    case CODE_JMP: // jump to an adress
        if (print)
        {
            std::stringstream debugStr;
            debugStr << ansi(YELLOW_FG) << "|     CODE_JMP: jump to " << std::hex << std::setfill('0') << std::setw(4) << adrJmp << ansi(RESET);
            printDebug(debugStr.str());
        }
        cpu->reg[J1] = ((cpu->pc & 0xff00) >> 8);
        cpu->reg[J2] = (cpu->pc & 0xff);
        cpu->pc = adrJmp;
        return getBlock(cpu->pc);

    case CODE_NXT: // executing the next block
        if (print)
        {
            std::stringstream debugStr;
            debugStr << ansi(YELLOW_FG) << "|     CODE_NXT: procede to next block at " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << ansi(RESET);
            printDebug(debugStr.str());
        }

        return getBlock(cpu->pc);

    case CODE_SET: // execute SET instruction
        if (print)
        {
            std::stringstream debugStr;
            debugStr << ansi(YELLOW_FG) << "|     CODE_SET: set val at " << std::hex << std::setfill('0') << std::setw(4) << adrJmp << " to " << std::hex << std::setfill('0') << std::setw(2) << (int)val << ansi(RESET);
            printDebug(debugStr.str());
        }

        if (rawBus)
        {
            cpu->setBusData(adrJmp, val);
        }
        else
        {
            cpu->set(adrJmp, val);
        }

        deleteBlocks(adrJmp);
        return getBlock(cpu->pc);

    case CODE_GET: // execute GET instruction
        if (print)
        {
            std::stringstream debugStr;
            debugStr << ansi(YELLOW_FG) << "|     CODE_GET: get val " << std::hex << std::setfill('0') << std::setw(2) << (int)val << " at " << std::hex << std::setfill('0') << std::setw(4) << adrJmp << " to reg[" << std::hex << (int)val << "]" << ansi(RESET);
            printDebug(debugStr.str());
        }

        if (rawBus)
        {
            cpu->reg[val] = cpu->getBusData(adrJmp);
        }
        else
        {
            cpu->reg[val] = cpu->get(adrJmp);
        }

        return getBlock(cpu->pc);

    default:
        printError("No Handler");
        running = false;
        break;
    }
    return nullptr;
}

void dynarec::Translater::initStep(uint16_t pc)
{
    printDebug("Translater: init");

    // setup variables
    cpu->pc = pc;
    startTime = std::chrono::steady_clock::now();
    running = true;
    lastHz = cpu->hz;
    deleteBlocks();

    // set a low blockSize if the Hz is low
    if (cpu->hz != 0)
    {
        blockSize = std::min((uint32_t)MAX_BLOCK_SIZE, cpu->hz);
    }
    else
    {
        blockSize = MAX_BLOCK_SIZE;
    }

    // get the current block
    e = getBlock(cpu->pc);
}

void dynarec::Translater::waitInst()
{
    if (cpu->hz != 0)
    {
        uint32_t cpuHz = cpu->hz;
        // if the cpu frequency has change
        if (lastHz != cpuHz)
        {
            if (print)
                printDebug(ansi(WHITE_FG) + "reset time");

            // reset blocks
            std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
            std::chrono::nanoseconds timeCpu((cpu->cycle * 1000000000) / cpuHz);

            startTime = timeNow - timeCpu;
            //cpu->cycle = 0;
            blockSize = std::min((uint32_t)MAX_BLOCK_SIZE, cpuHz);
            deleteBlocks();
            e = getBlock(cpu->pc);
        }

        // update lastHz
        lastHz = cpuHz;

        // find the time since the start (or the last reset or the last hz change)
        std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
        std::chrono::duration<long long, std::nano> timeSinceStart = std::chrono::duration_cast<std::chrono::duration<long long, std::nano>>(timeNow - startTime);

        // find the time that had past base in "instruction time"
        uint64_t nbInstTime = (timeSinceStart.count()) / (1000000000 / cpuHz);

        // print debug the isntructions time
        if (print)
        {
            std::stringstream debugStr;
            debugStr.str("");
            debugStr << ansi(WHITE_FG) << "nbInstTime: " << std::dec << nbInstTime << "  cycle:" << cpu->cycle;
            printDebug(debugStr.str());
        }

        // if we had execute too many instruction compare to the total "instruction time"
        if (cpu->cycle > nbInstTime)
        {
            // then find the time to wait (the number of cycle - the total "instruction time")
            std::chrono::nanoseconds waiting((cpu->cycle - nbInstTime) * (1000000000 / cpuHz));

            // print debug how many nansecond we wait
            if (print)
            {
                std::stringstream debugStr;
                debugStr.str("");
                debugStr << ansi(WHITE_FG) << "wait: " << waiting.count() << "ns";
                printDebug(debugStr.str());
            }

            // wait
            std::this_thread::sleep_for(waiting);
        }
    }
}

int dynarec::Translater::runStep()
{
    // if translater not running do nothing
    if (!running)
        return 0;

    // if the translater is paused
    if (pause)
    {
        // wait a moment
        std::chrono::nanoseconds timeWait(1000000);
        std::this_thread::sleep_for(timeWait);
        // and then do nothing
        return 0;
    }

    // setup variables
    int res = 0;

    // timing to wait between each block
    waitInst();

    // print debug what adr we will run
    if (print)
    {
        std::stringstream debugStr;
        debugStr.str("");
        debugStr << ansi(WHITE_FG) << "| run adr " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << " ...";
        printDebug(debugStr.str());
    }

    if (e != nullptr)
    {
        // print debug what block we execute
        if (print)
        {
            std::stringstream debugStr;
            debugStr.str("");
            debugStr << ansi(WHITE_FG) << "|     Execute block adr " << std::hex << std::setfill('0') << std::setw(4) << e->getStartAdr() << " with " << e->getInsCount() << " ins";
            printDebug(debugStr.str());
        }

        //execute the block
        res = e->execute();

        // update cpu registers
        uint16_t insCount = e->getInsCount();
        cpu->pc += insCount * 4;
        cpu->cycle += insCount;
        cpu->refreshCycle(0);
    }
    else
    {
        // we do not have an Emitter to execute so send an error code
        res = CODE_ERR;

        // print debug that we have no Emitter
        if (print)
            printDebug(ansi(RED_FG) + "| /!\\ ERROR: Emitter null" + ansi(RESET));
    }

    // print the cpu state after the block execution
    if (print)
        printCPUState();

    // handle the response of the block
    e = handlerEndBlock(res);

    // is their a breakpoint at the next instruction ?
    uint16_t cpuPC = cpu->pc;
    for (auto &&b : breakpoints)
    {
        if (b == cpuPC || b == cpuPC + 1 || b == cpuPC + 2 || b == cpuPC + 3)
        {
            if (print)
            {
                std::stringstream debugStr;
                debugStr << ansi(WHITE_FG) << "| breakpoint find at adr " << std::hex << std::setfill('0') << std::setw(4) << b << ", pausing execution" << ansi(RESET);
            }
            pause = true;
            break;
        }
    }

    return res;
}

int dynarec::Translater::run(uint16_t pc)
{
    printDebug("Translater: start running");
    // init the Translater
    int res = 0;
    initStep(pc);

    // while the translater is running
    while (running)
    {
        res = runStep();
    }

    printDebug("Translater: stop running");
    // return the last code from the Translater
    return res;
}

dynarec::Emitter *dynarec::Translater::getBlock(uint16_t pc)
{
    // if the block we want does not exist
    if (blocks[pc] == nullptr)
    {
        // then we recompile it
        recompile(pc);
    }

    // return the wanted block
    return blocks[pc];
}

void dynarec::Translater::printCPUState()
{
    // debug string
    std::stringstream debugStr;

    // general info
    debugStr << ansi(CYAN_FG) << "| cpu:";
    printDebug(debugStr.str());
    debugStr.str("");
    debugStr << ansi(CYAN_FG) << "|   cycle=" << std::dec << std::setfill('0') << std::setw(8) << cpu->cycle << "  pc=" << std::hex << std::setfill('0') << std::setw(4) << cpu->pc;
    printDebug(debugStr.str());

    // registers names
    debugStr.str("");
    debugStr << ansi(CYAN_FG) << "|   reg O   A   B   C   D   E   F   R   J1  J2  G0  G1  G2  G3  G4  G5";
    printDebug(debugStr.str());

    // registers values
    debugStr.str("");
    debugStr << ansi(CYAN_FG) << "|       ";
    for (unsigned int i = 0; i < 16; i++)
    {
        debugStr << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[i] << "  ";
    }

    // print debug the cpu state
    printDebug(debugStr.str());
}

void dynarec::Translater::setPause(bool pause)
{
    this->pause = pause;
    if (!this->pause)
    {
        std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
        std::chrono::nanoseconds timeCpu((cpu->cycle * 1000000000) / cpu->hz);
        startTime = timeNow - timeCpu;
    }
}

bool dynarec::Translater::getPause()
{
    return pause;
}

void dynarec::Translater::stepOnce()
{
    // if the translater is not running or paused, do nothing
    if (!running || !pause)
        return;

    // setup res
    int res = 0;

    // print debug what adr we will run
    if (print)
    {
        std::stringstream debugStr;
        debugStr.str("");
        debugStr << ansi(WHITE_FG) << "| run adr " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << " ...";
        printDebug(debugStr.str());
    }

    deleteBlocks(cpu->pc);
    e = getBlock(cpu->pc);

    // print debug what block we execute
    if (print)
    {
        std::stringstream debugStr;
        debugStr.str("");
        debugStr << ansi(WHITE_FG) << "|     Execute block adr " << std::hex << std::setfill('0') << std::setw(4) << e->getStartAdr() << " with " << e->getInsCount() << " ins";
        printDebug(debugStr.str());
    }

    //execute the block
    res = e->execute();

    // update cpu registers
    uint16_t insCount = e->getInsCount();
    cpu->pc += insCount * 4;
    cpu->cycle += insCount;
    cpu->refreshCycle(0);

    // print the cpu state after the block execution
    if (print)
        printCPUState();

    // handle the response of the block
    e = handlerEndBlock(res);

    return;
}

void dynarec::Translater::setBreakpoint(uint16_t adr)
{
    std::stringstream debugStr;
    debugStr << ansi(WHITE_FG) << "Add breakpoint at adress " << std::hex << std::setfill('0') << std::setw(4) << adr << ansi(RESET);
    printDebug(debugStr.str());
    breakpoints.push_back(adr);
    deleteBlocks(adr);
    e = getBlock(cpu->pc);
}

void dynarec::Translater::removeBreakpoint(uint16_t adr)
{
    int i = 0;
    for (auto &&b : breakpoints)
    {
        if (b == adr)
        {
            std::stringstream debugStr;
            debugStr << ansi(WHITE_FG) << "Remove breakpoint at adress " << std::hex << std::setfill('0') << std::setw(4) << adr << ansi(RESET);
            printDebug(debugStr.str());
            breakpoints.erase(breakpoints.begin() + i);
        }
        i++;
    }
}

std::vector<uint16_t> dynarec::Translater::getBreakpoints()
{
    return breakpoints;
}

void dynarec::Translater::recompile(uint16_t pc)
{
    std::stringstream debugStr;
    // print debug what block we want to recompile
    if (print)
    {
        debugStr << ansi(WHITE_FG) << "| recompile adr " << std::hex << std::setfill('0') << std::setw(4) << pc << " ...";
        printDebug(debugStr.str());
    }

    // if the block already exist
    if (blocks[pc] != nullptr)
    {
        // then delete it
        delete blocks[pc];
    }

    // get a new Emitter (block)
#ifndef _WIN32
    blocks[pc] = new Emitter64(cpu, pc);
#else
    blocks[pc] = new Emitter86(cpu, pc);
#endif
    Emitter *emitter = blocks[pc];

    // while we need to recompile
    bool recompile = true;
    while (recompile)
    {
        uint8_t ins = 0;
        uint8_t dst = 0;
        uint8_t src = 0;
        uint8_t val = 0;

        // get the bus data at the current pc adress
        if (rawBus)
        {
            ins = cpu->getBusData(pc);
            dst = cpu->getBusData(pc + 1);
            src = cpu->getBusData(pc + 2);
            val = cpu->getBusData(pc + 3);
        }
        else
        {
            ins = cpu->get(pc);
            dst = cpu->get(pc + 1);
            src = cpu->get(pc + 2);
            val = cpu->get(pc + 3);
        }

        // what instruction we need to emit
        switch (ins)
        {
        case NOP:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: NOP ");
            emitter->NOP();
            break;
        case OFF:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: OFF ");
            emitter->OFF();
            recompile = false;
            break;
        case MOV_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MOV_RV ");
            emitter->MOV((REG)dst, src);
            break;
        case MOV_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MOV_RR ");
            emitter->MOV((REG)dst, (REG)src);
            break;
        case CMP_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: CMP_RR ");
            emitter->CMP((REG)src, (REG)val);
            break;
        case CMP_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: CMP_RV ");
            emitter->CMP((REG)src, val);
            break;
        case CMP_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: CMP_VR ");
            emitter->CMP(src, (REG)val);
            break;
        case CMP_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: CMP_VV ");
            emitter->CMP(src, val);
            break;
        case ADD_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADD_RR ");
            emitter->ADD((REG)dst, (REG)src, (REG)val);
            break;
        case ADD_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADD_VR ");
            emitter->ADD((REG)dst, src, (REG)val);
            break;
        case ADD_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADD_RV ");
            emitter->ADD((REG)dst, (REG)src, val);
            break;
        case ADD_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADD_VV ");
            emitter->ADD((REG)dst, src, val);
            break;
        case ADC_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADC_RR ");
            emitter->ADC((REG)dst, (REG)src, (REG)val);
            break;
        case ADC_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADC_VR ");
            emitter->ADC((REG)dst, src, (REG)val);
            break;
        case ADC_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADC_RV ");
            emitter->ADC((REG)dst, (REG)src, val);
            break;
        case ADC_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: ADC_VV ");
            emitter->ADC((REG)dst, src, val);
            break;
        case SUB_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SUB_RR ");
            emitter->SUB((REG)dst, (REG)src, (REG)val);
            break;
        case SUB_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SUB_VR ");
            emitter->SUB((REG)dst, src, (REG)val);
            break;
        case SUB_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SUB_RV ");
            emitter->SUB((REG)dst, (REG)src, val);
            break;
        case SUB_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SUB_VV ");
            emitter->SUB((REG)dst, src, val);
            break;
        case SBB_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SBB_RR ");
            emitter->SBB((REG)dst, (REG)src, (REG)val);
            break;
        case SBB_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SBB_VR ");
            emitter->SBB((REG)dst, src, (REG)val);
            break;
        case SBB_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SBB_RV ");
            emitter->SBB((REG)dst, (REG)src, val);
            break;
        case SBB_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SBB_VV ");
            emitter->SBB((REG)dst, src, val);
            break;
        case MUL_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MUL_RR ");
            emitter->MUL((REG)dst, (REG)src, (REG)val);
            break;
        case MUL_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MUL_VR ");
            emitter->MUL((REG)dst, src, (REG)val);
            break;
        case MUL_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MUL_RV ");
            emitter->MUL((REG)dst, (REG)src, val);
            break;
        case MUL_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MUL_VV ");
            emitter->MUL((REG)dst, src, val);
            break;
        case DIV_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: DIV_RR ");
            emitter->DIV((REG)dst, (REG)src, (REG)val);
            break;
        case DIV_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: DIV_VR ");
            emitter->DIV((REG)dst, src, (REG)val);
            break;
        case DIV_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: DIV_RV ");
            emitter->DIV((REG)dst, (REG)src, val);
            break;
        case DIV_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: DIV_VV ");
            emitter->DIV((REG)dst, src, val);
            break;
        case MOD_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MOD_RR ");
            emitter->MOD((REG)dst, (REG)src, (REG)val);
            break;
        case MOD_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MOD_VR ");
            emitter->MOD((REG)dst, src, (REG)val);
            break;
        case MOD_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MOD_RV ");
            emitter->MOD((REG)dst, (REG)src, val);
            break;
        case MOD_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: MOD_VV ");
            emitter->MOD((REG)dst, src, val);
            break;
        case AND_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: AND_RR ");
            emitter->AND((REG)dst, (REG)src, (REG)val);
            break;
        case AND_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: AND_VR ");
            emitter->AND((REG)dst, src, (REG)val);
            break;
        case AND_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: AND_RV ");
            emitter->AND((REG)dst, (REG)src, val);
            break;
        case AND_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: AND_VV ");
            emitter->AND((REG)dst, src, val);
            break;
        case OR_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: OR_RR ");
            emitter->OR((REG)dst, (REG)src, (REG)val);
            break;
        case OR_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: OR_VR ");
            emitter->OR((REG)dst, src, (REG)val);
            break;
        case OR_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: OR_RV ");
            emitter->OR((REG)dst, (REG)src, val);
            break;
        case OR_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: OR_VV ");
            emitter->OR((REG)dst, src, val);
            break;
        case XOR_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: XOR_RR ");
            emitter->XOR((REG)dst, (REG)src, (REG)val);
            break;
        case XOR_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: XOR_VR ");
            emitter->XOR((REG)dst, src, (REG)val);
            break;
        case XOR_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: XOR_RV ");
            emitter->XOR((REG)dst, (REG)src, val);
            break;
        case XOR_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: XOR_VV ");
            emitter->XOR((REG)dst, src, val);
            break;
        case JMP_RRR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_RRR ");
            recompile = false;
            emitter->JMP((REG)dst, (REG)src, (REG)val);
            break;
        case JMP_RVR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_RVR ");
            recompile = false;
            emitter->JMP((REG)dst, src, (REG)val);
            break;
        case JMP_RRV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_RRV ");
            recompile = false;
            emitter->JMP((REG)dst, (REG)src, val);
            break;
        case JMP_RVV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_RVV ");
            recompile = false;
            emitter->JMP((REG)dst, src, val);
            break;
        case JMP_VRR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_VRR ");
            recompile = false;
            emitter->JMP(dst, (REG)src, (REG)val);
            break;
        case JMP_VVR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_VVR ");
            recompile = false;
            emitter->JMP(dst, src, (REG)val);
            break;
        case JMP_VRV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_VRV ");
            recompile = false;
            emitter->JMP(dst, (REG)src, val);
            break;
        case JMP_VVV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: JMP_VVV ");
            recompile = false;
            emitter->JMP(dst, src, val);
            break;
        case GET_RR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: GET_RR ");
            recompile = false;
            emitter->GET((REG)dst, (REG)src, (REG)val);
            break;
        case GET_VR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: GET_VR ");
            recompile = false;
            emitter->GET((REG)dst, src, (REG)val);
            break;
        case GET_RV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: GET_RV ");
            recompile = false;
            emitter->GET((REG)dst, (REG)src, val);
            break;
        case GET_VV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: GET_VV ");
            recompile = false;
            emitter->GET((REG)dst, src, val);
            break;
        case SET_RRR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_RRR ");
            recompile = false;
            emitter->SET((REG)dst, (REG)src, (REG)val);
            break;
        case SET_RVR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_RVR ");
            recompile = false;
            emitter->SET((REG)dst, src, (REG)val);
            break;
        case SET_RRV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_RRV ");
            recompile = false;
            emitter->SET((REG)dst, (REG)src, val);
            break;
        case SET_RVV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_RVV ");
            recompile = false;
            emitter->SET((REG)dst, src, val);
            break;
        case SET_VRR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_VRR ");
            recompile = false;
            emitter->SET(dst, (REG)src, (REG)val);
            break;
        case SET_VVR:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_VVR ");
            recompile = false;
            emitter->SET(dst, src, (REG)val);
            break;
        case SET_VRV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_VRV ");
            recompile = false;
            emitter->SET(dst, (REG)src, val);
            break;
        case SET_VVV:
            if (print)
                printDebug(ansi(WHITE_FG) + "|     Compile: SET_VVV ");
            recompile = false;
            emitter->SET(dst, src, val);
            break;
        default:
            debugStr.str("");
            debugStr << ansi(RED_FG) << "| /!\\ ERROR: no instruction with the opcode " << std::hex << (int)ins << ansi(RESET);
            printDebug(debugStr.str());
            break;
        }

        // increment the program counter to the next instruction
        pc += 4;

        // is their a breakpoint at the next instruction ?
        bool breakpointFound = false;
        for (auto &&b : breakpoints)
        {
            if (b == pc || b == pc + 1 || b == pc + 2 || b == pc + 3)
            {
                breakpointFound = true;
                break;
            }
        }

        // if we had compile too many instruction for one block
        if (emitter->getInsCount() >= blockSize || pause || breakpointFound)
        {
            if (print)
            {
                if (pause)
                {
                    printDebug(ansi(WHITE_FG) + "|     Compile: pause is active, stop ");
                }
                else if (breakpointFound)
                {
                    printDebug(ansi(WHITE_FG) + "|     Compile: breakpoint found, stop ");
                }
                else
                {
                    printDebug(ansi(WHITE_FG) + "|     Compile: block too big, stop ");
                }
            }
            emitter->NXT();
            recompile = false;
        }
    }

    if (print)
        printDebug(ansi(WHITE_FG) + "| recompile done");
}
