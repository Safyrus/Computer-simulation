#include "dynarecs/Translater.hpp"
#include "computer/instruction.h"
#include "utils/console.hpp"
#ifndef _WIN32
#include "dynarecs/Emitter64.hpp"
#else
#include "dynarecs/Emitter86.hpp"
#endif

#include <iostream>
#include <iomanip>
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
            std::cout << "delete block " << i << std::endl;
            delete blocks[i];
            blocks[i] = nullptr;
        }
    }
}


dynarec::Emitter *dynarec::Translater::handlerEndBlock(int ret)
{
    std::cout << ansi(YELLOW_FG) << "| handler end block" << std::endl;
    uint8_t val = ((ret & 0xff000000) >> 24);
    uint16_t adrJmp = ((ret & 0xffff00) >> 8);
    uint8_t code = (ret & 0xff);
    switch (code)
    {
    case CODE_RET:
        std::cout << "|     CODE_RET: stopping cpu" << ansi(RESET) << std::endl;
        running = false;
        break;
    case CODE_JMP:
        std::cout << "|     CODE_JMP: jump to " << std::hex << std::setfill('0') << std::setw(4) << adrJmp << ansi(RESET) << std::endl;
        cpu->reg[J1] = ((cpu->pc & 0xff00) >> 8);
        cpu->reg[J2] = (cpu->pc & 0xff);
        cpu->pc = adrJmp;
        return getBlock(cpu->pc);
    case CODE_NXT:
        std::cout << "|     CODE_NXT: procede to next block at " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << ansi(RESET) << std::endl;
        return getBlock(cpu->pc);
    case CODE_SET:
        std::cout << "|     CODE_SET: set val at " << std::hex << std::setfill('0') << std::setw(4) << adrJmp << " to " << std::hex << std::setfill('0') << std::setw(2) << val << ansi(RESET) << std::endl;
        cpu->set(adrJmp, val);
        return getBlock(cpu->pc);
    case CODE_GET:
        std::cout << "|     CODE_GET: get val at " << std::hex << std::setfill('0') << std::setw(4) << adrJmp << " to reg[" << std::hex << (int)val << "]" << ansi(RESET) << std::endl;
        cpu->reg[val] = cpu->get(adrJmp);
        return getBlock(cpu->pc);
    default:
        std::cout << ansi(RED_FG) << "| /!\\ ERROR: No Handler" << ansi(RESET) << std::endl;
        running = false;
        break;
    }
    return nullptr;
}

int dynarec::Translater::run(uint16_t pc)
{
    cpu->pc = pc;
    Emitter *e = getBlock(cpu->pc);
    running = true;
    int res = 0;
    startTime = std::chrono::steady_clock::now();
    uint32_t lastHz = cpu->hz;
    if (cpu->hz != 0)
    {
        blockSize = std::min((uint32_t)MAX_BLOCK_SIZE, std::max(cpu->hz/4, (unsigned)1));
    }
    while (running)
    {
        if (cpu->hz != 0)
        {
            if (lastHz != cpu->hz)
            {
                startTime = std::chrono::steady_clock::now();
                cpu->cycle = 0;
                std::cout << "reset time" << std::endl;
                blockSize = std::min((uint32_t)MAX_BLOCK_SIZE, cpu->hz);
                deleteBlocks();
                e = getBlock(cpu->pc);
            }
            lastHz = cpu->hz;

            std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
            std::chrono::duration<long long, std::nano> timeSinceStart = std::chrono::duration_cast<std::chrono::duration<long long, std::nano>>(timeNow - startTime);
            uint64_t nbInstTime = (timeSinceStart.count()) / (1000000000 / cpu->hz);

            std::cout << "nbInstTime: " << std::dec << nbInstTime << "  cycle:" << cpu->cycle << std::endl;
            if (cpu->cycle > nbInstTime)
            {
                std::chrono::nanoseconds waiting((cpu->cycle - nbInstTime) * (1000000000 / cpu->hz));
                std::cout << "wait: " << waiting.count() << "ns" << std::endl;
                std::this_thread::sleep_for(waiting);
            }
        }

        std::cout << "| run adr " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << " ..." << std::endl;
        if (e != nullptr)
        {
            std::cout << "|     Execute block adr " << std::hex << std::setfill('0') << std::setw(4) << e->getStartAdr() << " with " << e->getInsCount() << " ins" << std::endl;
            res = e->execute();
            cpu->pc += (e->getInsCount()) * 4;
            cpu->cycle += e->getInsCount();
        }
        else
        {
            std::cout << ansi(RED_FG) << "| /!\\ ERROR: Emitter null" << ansi(RESET) << std::endl;
            res = CODE_ERR;
        }
        printCPUState();
        e = handlerEndBlock(res);
    }
    return res;
}

dynarec::Emitter *dynarec::Translater::getBlock(uint16_t pc)
{
    if (blocks[pc] == nullptr)
    {
        recompile(pc);
    }
    return blocks[pc];
}

void dynarec::Translater::recompile(uint16_t pc)
{
    std::cout << "| recompile adr " << std::hex << std::setfill('0') << std::setw(4) << pc << " ..." << std::endl;
    if (blocks[pc] != nullptr)
    {
        delete blocks[pc];
    }
#ifndef _WIN32
    blocks[pc] = new Emitter64(cpu, pc);
#else
    blocks[pc] = new Emitter86(cpu, pc);
#endif
    Emitter *emitter = blocks[pc];
    bool recompile = true;
    while (recompile)
    {
        uint8_t ins = 0;
        uint8_t dst = 0;
        uint8_t src = 0;
        uint8_t val = 0;
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
        switch (ins)
        {
        case NOP:
            std::cout << "|     Compile: NOP " << std::endl;
            emitter->NOP();
            break;
        case OFF:
            std::cout << "|     Compile: OFF " << std::endl;
            emitter->OFF();
            recompile = false;
            break;
        case MOV_RV:
            std::cout << "|     Compile: MOV_RV " << std::endl;
            emitter->MOV((REG)dst, src);
            break;
        case MOV_RR:
            std::cout << "|     Compile: MOV_RR " << std::endl;
            emitter->MOV((REG)dst, (REG)src);
            break;
        case CMP_RR:
            std::cout << "|     Compile: CMP_RR " << std::endl;
            emitter->CMP((REG)src, (REG)val);
            break;
        case CMP_RV:
            std::cout << "|     Compile: CMP_RV " << std::endl;
            emitter->CMP((REG)src, val);
            break;
        case CMP_VR:
            std::cout << "|     Compile: CMP_VR " << std::endl;
            emitter->CMP(src, (REG)val);
            break;
        case CMP_VV:
            std::cout << "|     Compile: CMP_VV " << std::endl;
            emitter->CMP(src, val);
            break;
        case ADD_RR:
            std::cout << "|     Compile: ADD_RR " << std::endl;
            emitter->ADD((REG)dst, (REG)src, (REG)val);
            break;
        case ADD_VR:
            std::cout << "|     Compile: ADD_VR " << std::endl;
            emitter->ADD((REG)dst, src, (REG)val);
            break;
        case ADD_RV:
            std::cout << "|     Compile: ADD_RV " << std::endl;
            emitter->ADD((REG)dst, (REG)src, val);
            break;
        case ADD_VV:
            std::cout << "|     Compile: ADD_VV " << std::endl;
            emitter->ADD((REG)dst, src, val);
            break;
        case ADC_RR:
            std::cout << "|     Compile: ADC_RR " << std::endl;
            emitter->ADC((REG)dst, (REG)src, (REG)val);
            break;
        case ADC_VR:
            std::cout << "|     Compile: ADC_VR " << std::endl;
            emitter->ADC((REG)dst, src, (REG)val);
            break;
        case ADC_RV:
            std::cout << "|     Compile: ADC_RV " << std::endl;
            emitter->ADC((REG)dst, (REG)src, val);
            break;
        case ADC_VV:
            std::cout << "|     Compile: ADC_VV " << std::endl;
            emitter->ADC((REG)dst, src, val);
            break;
        case SUB_RR:
            std::cout << "|     Compile: SUB_RR " << std::endl;
            emitter->SUB((REG)dst, (REG)src, (REG)val);
            break;
        case SUB_VR:
            std::cout << "|     Compile: SUB_VR " << std::endl;
            emitter->SUB((REG)dst, src, (REG)val);
            break;
        case SUB_RV:
            std::cout << "|     Compile: SUB_RV " << std::endl;
            emitter->SUB((REG)dst, (REG)src, val);
            break;
        case SUB_VV:
            std::cout << "|     Compile: SUB_VV " << std::endl;
            emitter->SUB((REG)dst, src, val);
            break;
        case SBB_RR:
            std::cout << "|     Compile: SBB_RR " << std::endl;
            emitter->SBB((REG)dst, (REG)src, (REG)val);
            break;
        case SBB_VR:
            std::cout << "|     Compile: SBB_VR " << std::endl;
            emitter->SBB((REG)dst, src, (REG)val);
            break;
        case SBB_RV:
            std::cout << "|     Compile: SBB_RV " << std::endl;
            emitter->SBB((REG)dst, (REG)src, val);
            break;
        case SBB_VV:
            std::cout << "|     Compile: SBB_VV " << std::endl;
            emitter->SBB((REG)dst, src, val);
            break;
        case MUL_RR:
            std::cout << "|     Compile: MUL_RR " << std::endl;
            emitter->MUL((REG)dst, (REG)src, (REG)val);
            break;
        case MUL_VR:
            std::cout << "|     Compile: MUL_VR " << std::endl;
            emitter->MUL((REG)dst, src, (REG)val);
            break;
        case MUL_RV:
            std::cout << "|     Compile: MUL_RV " << std::endl;
            emitter->MUL((REG)dst, (REG)src, val);
            break;
        case MUL_VV:
            std::cout << "|     Compile: MUL_VV " << std::endl;
            emitter->MUL((REG)dst, src, val);
            break;
        case DIV_RR:
            std::cout << "|     Compile: DIV_RR " << std::endl;
            emitter->DIV((REG)dst, (REG)src, (REG)val);
            break;
        case DIV_VR:
            std::cout << "|     Compile: DIV_VR " << std::endl;
            emitter->DIV((REG)dst, src, (REG)val);
            break;
        case DIV_RV:
            std::cout << "|     Compile: DIV_RV " << std::endl;
            emitter->DIV((REG)dst, (REG)src, val);
            break;
        case DIV_VV:
            std::cout << "|     Compile: DIV_VV " << std::endl;
            emitter->DIV((REG)dst, src, val);
            break;
        case MOD_RR:
            std::cout << "|     Compile: MOD_RR " << std::endl;
            emitter->MOD((REG)dst, (REG)src, (REG)val);
            break;
        case MOD_VR:
            std::cout << "|     Compile: MOD_VR " << std::endl;
            emitter->MOD((REG)dst, src, (REG)val);
            break;
        case MOD_RV:
            std::cout << "|     Compile: MOD_RV " << std::endl;
            emitter->MOD((REG)dst, (REG)src, val);
            break;
        case MOD_VV:
            std::cout << "|     Compile: MOD_VV " << std::endl;
            emitter->MOD((REG)dst, src, val);
            break;
        case AND_RR:
            std::cout << "|     Compile: AND_RR " << std::endl;
            emitter->AND((REG)dst, (REG)src, (REG)val);
            break;
        case AND_VR:
            std::cout << "|     Compile: AND_VR " << std::endl;
            emitter->AND((REG)dst, src, (REG)val);
            break;
        case AND_RV:
            std::cout << "|     Compile: AND_RV " << std::endl;
            emitter->AND((REG)dst, (REG)src, val);
            break;
        case AND_VV:
            std::cout << "|     Compile: AND_VV " << std::endl;
            emitter->AND((REG)dst, src, val);
            break;
        case OR_RR:
            std::cout << "|     Compile: OR_RR " << std::endl;
            emitter->OR((REG)dst, (REG)src, (REG)val);
            break;
        case OR_VR:
            std::cout << "|     Compile: OR_VR " << std::endl;
            emitter->OR((REG)dst, src, (REG)val);
            break;
        case OR_RV:
            std::cout << "|     Compile: OR_RV " << std::endl;
            emitter->OR((REG)dst, (REG)src, val);
            break;
        case OR_VV:
            std::cout << "|     Compile: OR_VV " << std::endl;
            emitter->OR((REG)dst, src, val);
            break;
        case XOR_RR:
            std::cout << "|     Compile: XOR_RR " << std::endl;
            emitter->XOR((REG)dst, (REG)src, (REG)val);
            break;
        case XOR_VR:
            std::cout << "|     Compile: XOR_VR " << std::endl;
            emitter->XOR((REG)dst, src, (REG)val);
            break;
        case XOR_RV:
            std::cout << "|     Compile: XOR_RV " << std::endl;
            emitter->XOR((REG)dst, (REG)src, val);
            break;
        case XOR_VV:
            std::cout << "|     Compile: XOR_VV " << std::endl;
            emitter->XOR((REG)dst, src, val);
            break;
        case JMP_RRR:
            std::cout << "|     Compile: JMP_RRR " << std::endl;
            recompile = false;
            emitter->JMP((REG)dst, (REG)src, (REG)val);
            break;
        case JMP_RVR:
            std::cout << "|     Compile: JMP_RVR " << std::endl;
            recompile = false;
            emitter->JMP((REG)dst, src, (REG)val);
            break;
        case JMP_RRV:
            std::cout << "|     Compile: JMP_RRV " << std::endl;
            recompile = false;
            emitter->JMP((REG)dst, (REG)src, val);
            break;
        case JMP_RVV:
            std::cout << "|     Compile: JMP_RVV " << std::endl;
            recompile = false;
            emitter->JMP((REG)dst, src, val);
            break;
        case JMP_VRR:
            std::cout << "|     Compile: JMP_VRR " << std::endl;
            recompile = false;
            emitter->JMP(dst, (REG)src, (REG)val);
            break;
        case JMP_VVR:
            std::cout << "|     Compile: JMP_VVR " << std::endl;
            recompile = false;
            emitter->JMP(dst, src, (REG)val);
            break;
        case JMP_VRV:
            std::cout << "|     Compile: JMP_VRV " << std::endl;
            recompile = false;
            emitter->JMP(dst, (REG)src, val);
            break;
        case JMP_VVV:
            std::cout << "|     Compile: JMP_VVV " << std::endl;
            recompile = false;
            emitter->JMP(dst, src, val);
            break;
        case GET_RR:
            std::cout << "|     Compile: GET_RR " << std::endl;
            recompile = false;
            emitter->GET((REG)dst, (REG)src, (REG)val);
            break;
        case GET_VR:
            std::cout << "|     Compile: GET_VR " << std::endl;
            recompile = false;
            emitter->GET((REG)dst, src, (REG)val);
            break;
        case GET_RV:
            std::cout << "|     Compile: GET_RV " << std::endl;
            recompile = false;
            emitter->GET((REG)dst, (REG)src, val);
            break;
        case GET_VV:
            std::cout << "|     Compile: GET_VV " << std::endl;
            recompile = false;
            emitter->GET((REG)dst, src, val);
            break;
        case SET_RRR:
            std::cout << "|     Compile: SET_RRR " << std::endl;
            recompile = false;
            emitter->SET((REG)dst, (REG)src, (REG)val);
            break;
        case SET_RVR:
            std::cout << "|     Compile: SET_RVR " << std::endl;
            recompile = false;
            emitter->SET((REG)dst, src, (REG)val);
            break;
        case SET_RRV:
            std::cout << "|     Compile: SET_RRV " << std::endl;
            recompile = false;
            emitter->SET((REG)dst, (REG)src, val);
            break;
        case SET_RVV:
            std::cout << "|     Compile: SET_RVV " << std::endl;
            recompile = false;
            emitter->SET((REG)dst, src, val);
            break;
        case SET_VRR:
            std::cout << "|     Compile: SET_VRR " << std::endl;
            recompile = false;
            emitter->SET(dst, (REG)src, (REG)val);
            break;
        case SET_VVR:
            std::cout << "|     Compile: SET_VVR " << std::endl;
            recompile = false;
            emitter->SET(dst, src, (REG)val);
            break;
        case SET_VRV:
            std::cout << "|     Compile: SET_VRV " << std::endl;
            recompile = false;
            emitter->SET(dst, (REG)src, val);
            break;
        case SET_VVV:
            std::cout << "|     Compile: SET_VVV " << std::endl;
            recompile = false;
            emitter->SET(dst, src, val);
            break;
        default:
            std::cout << ansi(RED_FG) << "| /!\\ ERROR: no instruction with the opcode " << std::hex << (int)ins << ansi(RESET) << std::endl;
            break;
        }
        pc += 4;
        if (emitter->getInsCount() >= blockSize)
        {
            std::cout << "|     Compile: block too big, stop " << std::endl;
            emitter->NXT();
            recompile = false;
        }
    }

    std::cout << "| recompile done" << std::endl;
}

void dynarec::Translater::printCPUState()
{
    std::cout << ansi(CYAN_FG) << "| cpu:\n|   cycle=" << std::dec << std::setfill('0') << std::setw(8) << cpu->cycle << "  pc=" << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << std::endl;
    std::cout << "|   reg O   A   B   C   D   E   F   R   J1  J2  G0  G1  G2  G3  G4  G5\n|       ";
    for (unsigned int i = 0; i < 16; i++)
    {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[i] << "  ";
    }
    std::cout << ansi(RESET) << "\n";
}
