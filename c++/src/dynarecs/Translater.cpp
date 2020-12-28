#include "dynarecs/Translater.hpp"
#include "dynarecs/Emitter.hpp"
#include "computer/instruction.h"
#include "utils/console.hpp"

#include <iostream>
#include <iomanip>

dynarec::Translater::Translater(std::shared_ptr<computer::CPU> cpu, bool rawBus)
{
    this->cpu = cpu;
    this->rawBus = rawBus;
    running = false;
    for (int i = 0; i < 1024 * 64; i++)
    {
        blocks.push_back(nullptr);
    }
}

dynarec::Translater::~Translater()
{

    for (unsigned int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i] != nullptr)
        {
            delete blocks[i];
        }
    }
}

dynarec::Emitter *dynarec::Translater::handlerEndBlock(int ret)
{
    std::cout << ansi(YELLOW_FG) << "| handler end block" << ansi(RESET) << std::endl;
    uint16_t adrJmp = ((ret & 0xffff00) >> 8);
    uint8_t code = (ret & 0xff);
    switch (code)
    {
    case CODE_RET:
        running = false;
        std::cout << "|     CODE_RET: stopping cpu" << std::endl;
        break;
    case CODE_JMP:
        cpu->pc = adrJmp;
        std::cout << "|     CODE_JMP: jump to " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << std::endl;
        return getBlock(cpu->pc);
    case CODE_NXT:
        std::cout << "|     CODE_NXT: procede to next block at " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << std::endl;
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
    while (running)
    {
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
    blocks[pc] = new Emitter(cpu, pc);
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
        case JMP_RVV:
            std::cout << "|     Compile: JMP_RVV " << std::endl;
            recompile = false;
            emitter->JMP((REG)dst, src, val);
            break;
        default:
            std::cout << ansi(RED_FG) << "| /!\\ ERROR: no instruction with the opcode " << std::hex << (int)ins << ansi(RESET) << std::endl;
            break;
            // TODO
        }
        pc += 4;
    }

    std::cout << "| recompile done" << std::endl;
}

void dynarec::Translater::printCPUState()
{
    std::cout << ansi(CYAN_FG) << "| cpu:\n|   cycle=" << std::dec << std::setfill('0') << std::setw(8) << cpu->cycle << "  pc=" << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << std::endl;
    std::cout << "|   reg A   B   C   D   E   F\n|       ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[A] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[B] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[C] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[D] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[E] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[F] << ansi(RESET) << "\n";
}
