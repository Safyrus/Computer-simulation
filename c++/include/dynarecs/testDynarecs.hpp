#ifndef TESTDYNARECS_HPP
#define TESTDYNARECS_HPP

#include "computer/CPU.hpp"
#include "computer/registers.h"
#include "computer/instruction.h"

#include "dynarecs/Buffer.hpp"
#include "dynarecs/Emitter.hpp"
#include "dynarecs/Translater.hpp"

#include "utils/hexTxtToBin.hpp"

void printCPU(std::shared_ptr<computer::CPU> cpu)
{
    std::cout << "\ncpu:\n  cycle=" << std::dec << std::setfill('0') << std::setw(8) << cpu->cycle << "  pc=" << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << std::endl;
    std::cout << "reg A   B   C   D   E   F\n    ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[A] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[B] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[C] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[D] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[E] << "  ";
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)cpu->reg[F] << "\n";
}

void printTranslaterResultCode(int res)
{
    std::cout << "result: " << std::hex << res;
    switch (res)
    {
    case dynarec::Translater::CODE_RET:
        std::cout << " (CODE_RET)" << std::endl;
        break;
    case dynarec::Translater::CODE_JMP:
        std::cout << " (CODE_JMP)" << std::endl;
        break;
    case dynarec::Translater::CODE_NXT:
        std::cout << " (CODE_NXT)" << std::endl;
        break;
    case dynarec::Translater::CODE_ERR:
        std::cout << " (CODE_ERR)" << std::endl;
        break;
    default:
        std::cout << " (\?\?\?)" << std::endl;
        break;
    }
}

void testBuffer()
{
    std::cout << "\nCreate buffer..." << std::endl;
    dynarec::Buffer buf(0x0000, 0);
    uint8_t *ptr = buf.getPtr();

    buf.write8(0xB8); // MOV value to eax
    buf.write32(0x12345678);
    //buf.write8(0x78); // value (xxxxxx78)
    //buf.write8(0x56); // value (xxxx56xx)
    //buf.write8(0x34); // value (xx34xxxx)
    //buf.write8(0x12); // value (12xxxxxx)
    buf.write8(0xC3); // ret
    buf.write8(0x74); // t
    buf.write8(0x65); // e
    buf.write8(0x73); // s
    buf.write8(0x74); // t
    buf.write8(0x20); //
    buf.write8(0x69); // i
    buf.write8(0x73); // s
    buf.write8(0x20); //
    buf.write8(0x67); // g
    buf.write8(0x6f); // o
    buf.write8(0x6f); // o
    buf.write8(0x64); // d

    std::cout << "read buffer..." << std::endl;
    unsigned int size = buf.getSize();
    if (!ptr)
    {
        ptr = buf.getPtr();
    }
    for (unsigned int i = 0; i < size; i++)
    {
        std::cout << (char)(*ptr % 0x7f);
        ptr++;
    }

    std::cout << "\nexecute buffer..." << std::endl;
    int res = buf.execute();
    std::cout << "result : " << std::hex << res << std::endl;
}

void testEmitter()
{
    std::cout << "\nCreate Emitter" << std::endl;
    std::shared_ptr<computer::CPU> cpu = std::make_shared<computer::CPU>();
    dynarec::Emitter e(cpu, 0x0000);

    std::cout << "Emit code" << std::endl;
    e.MOV(B, 0x17);
    e.NOP();
    e.MOV(C, 0x34);
    e.ADD(A, B, C);
    e.MOV(B, C);
    e.ADD(A, A, A);
    e.CMP(A, A);
    e.JMP(A, A, B);
    //e.OFF();

    std::cout << "Emitter instruction count: " << e.getInsCount() << std::endl;
    std::cout << "execute Emitter" << std::endl;
    printCPU(cpu);
}

void testTranslater()
{
    std::cout << "\nCreate Translater" << std::endl;
    std::shared_ptr<computer::CPU> cpu = std::make_shared<computer::CPU>();
    dynarec::Translater t(cpu);
    
    std::cout << "Update CPU bus" << std::endl;
    // MOV B 12
    cpu->setBusData(0x0000, MOV_RV);
    cpu->setBusData(0x0001, B);
    cpu->setBusData(0x0002, 0x12);
    cpu->setBusData(0x0003, 0x00);
    // JMP B A A
    cpu->setBusData(0x0004, JMP_RRR);
    cpu->setBusData(0x0005, B);
    cpu->setBusData(0x0006, A);
    cpu->setBusData(0x0007, B);
    // MOV A 34
    cpu->setBusData(0x0008, MOV_RV);
    cpu->setBusData(0x0009, A);
    cpu->setBusData(0x000A, 0x34);
    cpu->setBusData(0x000B, 0x00);
    // OFF
    cpu->setBusData(0x0010, OFF);
    cpu->setBusData(0x0011, 0x00);
    cpu->setBusData(0x0012, 0x00);
    cpu->setBusData(0x0013, 0x00);

    std::cout << "Execute Translater" << std::endl;
    int res = t.run(0x0004);
    printTranslaterResultCode(res);
    printCPU(cpu);
}

void testTranslater2()
{
    std::cout << "\nCreate Translater" << std::endl;
    std::shared_ptr<computer::CPU> cpu = std::make_shared<computer::CPU>();
    dynarec::Translater t(cpu);

    std::cout << "Create Data from file" << std::endl;
    std::vector<uint8_t> data = hexTxtToBin("bin/test");

    std::cout << "Load data on Bus" << std::endl;
    cpu->loadOnBus(0x0000, data);

    std::cout << "Execute Translater" << std::endl;
    int res = t.run(0x0004);
    printTranslaterResultCode(res);
    printCPU(cpu);
}

#endif // TESTDYNARECS_HPP