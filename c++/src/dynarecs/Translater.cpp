#include "dynarecs/Translater.hpp"
#include "dynarecs/Emitter.hpp"
#include "computer/instruction.h"

#include <iostream>
#include <iomanip>

dynarec::Translater::Translater(std::shared_ptr<computer::CPU> cpu)
{
    this->cpu = cpu;
    running = false;
    for (int i = 0; i < 1024*64; i++)
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
    std::cout << "| handler end block" << std::endl;
    uint16_t adrJmp = ((ret & 0xffff00)>>8);
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
        std::cout << "| /!\\ ERROR: No Handler" << std::endl;
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
        if(e != nullptr)
        {
            std::cout << "|     Execute block adr " << std::hex << std::setfill('0') << std::setw(4) << e->getStartAdr() << " with " << e->getInsCount() << " ins" << std::endl;
            res = e->execute();
            cpu->pc += (e->getInsCount())*4;
            cpu->cycle += e->getInsCount();
        }else 
        {
            std::cout << "| /!\\ ERROR: Emitter null" << std::endl;
            res = CODE_ERR;
        }
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
        uint8_t ins = cpu->getBusData(pc);
        uint8_t dst = cpu->getBusData(pc + 1);
        uint8_t src = cpu->getBusData(pc + 2);
        uint8_t val = cpu->getBusData(pc + 3);
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
        case JMP_RRR:
            std::cout << "|     Compile: JMP_RRR " << std::endl;
            recompile = false;
            emitter->JMP((REG)dst, (REG)src, (REG)val);
            break;
        default:
            std::cout << "| /!\\ ERROR: no instruction with the opcode " << std::hex << ins << std::endl;
            break;
        // TODO
        }
        pc += 4;
    }

    std::cout << "| recompile done" << std::endl;
    // TODO
}
