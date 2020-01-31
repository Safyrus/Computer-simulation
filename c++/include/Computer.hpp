#pragma once
#include "CPU.hpp"
#include "DISK.hpp"

class Computer
{
private:
    CPU *cpu;
    DISK *disk;

public:
    Computer(const char *f);
    ~Computer();
    void cycle();
    void setPwr();
    bool getPwr();
};