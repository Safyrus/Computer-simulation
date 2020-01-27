#include "CPU.hpp"
#include <iostream>

CPU::CPU()
{
    step = 0;
    this->clk = false;
    this->pwr = false;
    load = false;

    adr = 0;
    adrBack = 0;
    data = 0;
    dataBack = 0;
    for (int i = 0; i < REGMAX; i++)
    {
        reg[i] = 0;
    }
}

CPU::~CPU()
{
}

int CPU::getAdr()
{
    return adr;
}

bool CPU::getLoad()
{
    return load;
}

int CPU::getData()
{
    return data;
}

void CPU::setData(int d)
{
    data = d;
}

int CPU::getStep()
{
    return step;
}

bool CPU::getPwr()
{
    return pwr;
}

bool CPU::getClk()
{
    return this->clk;
}

void CPU::setClk()
{
    this->clk = !this->clk;
    step = 0;
}

void CPU::setRst()
{
    load = false;
    step = 0;

    adr = 0;
    adrBack = 0;
    data = 0;
    dataBack = 0;
    for (int i = 0; i < REGMAX; i++)
    {
        reg[i] = 0;
    }
}

void CPU::setPwr()
{
    this->pwr = !this->pwr;
    std::cout << "[CPU] : power=" << pwr << std::endl;
    setRst();
}

int CPU::CU()
{
    int cmd = data >> 24 & 0xff;
    int arg1 = data >> 16 & 0xff;
    int arg2 = data >> 8 & 0xff;
    int arg3 = data & 0xff;
    if (step != 0)
    {
        cmd = dataBack >> 24 & 0xff;
        arg1 = dataBack >> 16 & 0xff;
        arg2 = dataBack >> 8 & 0xff;
        arg3 = dataBack & 0xff;
    }
    std::cout << "[CU] : cmd=" << cmd << "  arg1=" << arg1 << "  arg2=" << arg2 << "  arg3=" << arg3 << std::endl;

    int res = 0;
    switch (cmd)
    {
    case 0x00:
        adr += 4;
        break;
    case 0x01:
        setRst();
        break;
    case 0x02:
        setPwr();
        break;
    case 0x03:
        reg[arg1] = reg[arg2];
        break;
    case 0x04:
        reg[arg1] = arg2;
        break;
    case 0x05:
        ALU(reg[arg2], reg[arg3], 0x1, true);
        break;
    case 0x06:
        ALU(arg2, reg[arg3], 0x1, true);
        break;
    case 0x07:
        ALU(reg[arg2], arg3, 0x1, true);
        break;
    case 0x08:
        ALU(arg2, arg3, 0x1, true);
        break;
    case 0x10:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x0, false);
        break;
    case 0x11:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x8, false);
        break;
    case 0x12:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x1, false);
        break;
    case 0x13:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x9, false);
        break;
    case 0x14:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x2, false);
        break;
    case 0x15:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x3, false);
        break;
    case 0x16:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x4, false);
        break;
    case 0x17:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x5, false);
        break;
    case 0x18:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x6, false);
        break;
    case 0x19:
        reg[arg1] = ALU(reg[arg2], reg[arg3], 0x7, false);
        break;
    case 0x20:
        reg[arg1] = ALU(arg2, reg[arg3], 0x0, false);
        break;
    case 0x21:
        reg[arg1] = ALU(arg2, reg[arg3], 0x8, false);
        break;
    case 0x22:
        reg[arg1] = ALU(arg2, reg[arg3], 0x1, false);
        break;
    case 0x23:
        reg[arg1] = ALU(arg2, reg[arg3], 0x9, false);
        break;
    case 0x24:
        reg[arg1] = ALU(arg2, reg[arg3], 0x2, false);
        break;
    case 0x25:
        reg[arg1] = ALU(arg2, reg[arg3], 0x3, false);
        break;
    case 0x26:
        reg[arg1] = ALU(arg2, reg[arg3], 0x4, false);
        break;
    case 0x27:
        reg[arg1] = ALU(arg2, reg[arg3], 0x5, false);
        break;
    case 0x28:
        reg[arg1] = ALU(arg2, reg[arg3], 0x6, false);
        break;
    case 0x29:
        reg[arg1] = ALU(arg2, reg[arg3], 0x7, false);
        break;
    case 0x30:
        reg[arg1] = ALU(reg[arg2], arg3, 0x0, false);
        break;
    case 0x31:
        reg[arg1] = ALU(reg[arg2], arg3, 0x8, false);
        break;
    case 0x32:
        reg[arg1] = ALU(reg[arg2], arg3, 0x1, false);
        break;
    case 0x33:
        reg[arg1] = ALU(reg[arg2], arg3, 0x9, false);
        break;
    case 0x34:
        reg[arg1] = ALU(reg[arg2], arg3, 0x2, false);
        break;
    case 0x35:
        reg[arg1] = ALU(reg[arg2], arg3, 0x3, false);
        break;
    case 0x36:
        reg[arg1] = ALU(reg[arg2], arg3, 0x4, false);
        break;
    case 0x37:
        reg[arg1] = ALU(reg[arg2], arg3, 0x5, false);
        break;
    case 0x38:
        reg[arg1] = ALU(reg[arg2], arg3, 0x6, false);
        break;
    case 0x39:
        reg[arg1] = ALU(reg[arg2], arg3, 0x7, false);
        break;
    case 0x40:
        reg[arg1] = ALU(arg2, arg3, 0x0, false);
        break;
    case 0x41:
        reg[arg1] = ALU(arg2, arg3, 0x8, false);
        break;
    case 0x42:
        reg[arg1] = ALU(arg2, arg3, 0x1, false);
        break;
    case 0x43:
        reg[arg1] = ALU(arg2, arg3, 0x9, false);
        break;
    case 0x44:
        reg[arg1] = ALU(arg2, arg3, 0x2, false);
        break;
    case 0x45:
        reg[arg1] = ALU(arg2, arg3, 0x3, false);
        break;
    case 0x46:
        reg[arg1] = ALU(arg2, arg3, 0x4, false);
        break;
    case 0x47:
        reg[arg1] = ALU(arg2, arg3, 0x5, false);
        break;
    case 0x48:
        reg[arg1] = ALU(arg2, arg3, 0x6, false);
        break;
    case 0x49:
        reg[arg1] = ALU(arg2, arg3, 0x7, false);
        break;
    case 0x50:
        if (reg[FLAG] == reg[arg1])
        {
            adrBack = adr;
            int jmp = (reg[arg2] << 8) + reg[arg3];
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x51:
        if (reg[FLAG] == reg[arg1])
        {
            adrBack = adr;
            int jmp = (arg2 << 8) + reg[arg3];
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x52:
        if (reg[FLAG] == reg[arg1])
        {
            adrBack = adr;
            int jmp = (reg[arg2] << 8) + arg3;
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x53:
        if (reg[FLAG] == reg[arg1])
        {
            adrBack = adr;
            int jmp = (arg2 << 8) + arg3;
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x54:
        if (reg[FLAG] == arg1)
        {
            adrBack = adr;
            int jmp = (reg[arg2] << 8) + reg[arg3];
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x55:
        if (reg[FLAG] == arg1)
        {
            adrBack = adr;
            int jmp = (arg2 << 8) + reg[arg3];
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x56:
        if (reg[FLAG] == arg1)
        {
            adrBack = adr;
            int jmp = (reg[arg2] << 8) + arg3;
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x57:
        if (reg[FLAG] == arg1)
        {
            adrBack = adr;
            int jmp = (arg2 << 8) + arg3;
            adr = jmp;
        }
        else
        {
            adr += 4;
        }
        break;
    case 0x60:
        switch (step)
        {
        case 0:
            load = true;
            dataBack = data;
            adrBack = adr;

            adr = (reg[arg2] << 8) + reg[arg3];
            step = 1;
            break;
        case 1:
            reg[arg1] = data;
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x61:
        switch (step)
        {
        case 0:
            load = true;
            dataBack = data;
            adrBack = adr;

            adr = (arg2 << 8) + reg[arg3];
            step = 1;
            break;
        case 1:
            reg[arg1] = data;
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x62:
        switch (step)
        {
        case 0:
            load = true;
            dataBack = data;
            adrBack = adr;

            adr = (reg[arg2] << 8) + arg3;
            step = 1;
            break;
        case 1:
            reg[arg1] = data;
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x63:
        switch (step)
        {
        case 0:
            load = true;
            dataBack = data;
            adrBack = adr;

            adr = (arg2 << 8) + arg3;
            step = 1;
            break;
        case 1:
            reg[arg1] = data;
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x64:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (reg[arg2] << 8) + reg[arg3];
            data = reg[arg1];
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x65:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (arg2 << 8) + reg[arg3];
            data = reg[arg1];
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x66:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (reg[arg2] << 8) + arg3;
            data = reg[arg1];
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x67:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (arg2 << 8) + arg3;
            data = reg[arg1];
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x68:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (reg[arg2] << 8) + reg[arg3];
            data = arg1;
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x69:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (arg2 << 8) + reg[arg3];
            data = arg1;
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x6a:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (reg[arg2] << 8) + arg3;
            data = arg1;
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    case 0x6b:
        switch (step)
        {
        case 0:
            load = false;
            dataBack = data;
            adrBack = adr;

            adr = (arg2 << 8) + arg3;
            data = arg1;
            step = 1;
            break;
        case 1:
            adr = adrBack;
            adr += 4;
            step = 2;
            break;
        default:
            break;
        }
        break;
    default:
        //no instruction found
        break;
    }
    if (cmd >= 0x03 && cmd <= 0x49)
    {
        adr += 4;
    }

    return res;
}

int CPU::ALU(int a, int b, int op, bool flag)
{
    bool carry = false;
    int res = 0;
    switch (op)
    {
    case 0x0:
        res = a + b;
        carry = (res > 127 || res < -128);
        break;
    case 0x8:
        res = a + b + 1;
        carry = (res > 127 || res < -128);
        break;
    case 0x1:
        res = a - b;
        carry = (res > 127 || res < -128);
        break;
    case 0x9:
        res = a - b - 1;
        carry = (res > 127 || res < -128);
        break;
    case 0x2:
        res = a * b;
        carry = (res > 127 || res < -128);
        break;
    case 0x3:
        if (b != 0)
        {
            res = a / b;
        }
        else
        {
            res = 0;
        }
        break;
    case 0x4:
        if (b != 0)
        {
            res = a % b;
        }
        else
        {
            res = 0;
        }
        break;
    case 0x5:
        res = a & b;
        break;
    case 0x6:
        res = a | b;
        break;
    case 0x7:
        res = a ^ b;
        break;
    default:
        //no operation found
        break;
    }
    if (flag)
    {
        if (a == b)
        {
            reg[FLAG] |= 0x04;
        }
        else
        {
            reg[FLAG] &= 0xfb;
        }
        if (a < b)
        {
            reg[FLAG] |= 0x08;
        }
        else
        {
            reg[FLAG] &= 0xf7;
        }
        if (a > b)
        {
            reg[FLAG] |= 0x02;
        }
        else
        {
            reg[FLAG] &= 0xfd;
        }
        if (carry)
        {
            reg[FLAG] |= 0x01;
        }
        else
        {
            reg[FLAG] &= 0xfe;
        }
    }
    return res;
}

void CPU::stp()
{
    if (pwr)
    {
        if (clk)
        {
            std::cout << "[CPU] : cu" << std::endl;
            CU();
        }
        else
        {
        }
    }
    reg[0] = 0;
    reg[7] = rand()&0xff;
    std::cout << "[CPU] : reg";
    for (int i = 0; i < REGMAX; i++)
    {
        std::cout << reg[i] << "  ";
    }
    std::cout << std::endl;
}
