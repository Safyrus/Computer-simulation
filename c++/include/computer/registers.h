#ifndef REGISTERS_H
#define REGISTERS_H

enum REG
{
    O  = 0,
    A  = 1,
    B  = 2,
    C  = 3,
    D  = 4,
    E  = 5,
    F  = 6,
    R  = 7,
    J1 = 8,
    J2 = 9,
    G0 = 10,
    G1 = 11,
    G2 = 12,
    G3 = 13,
    G4 = 14,
    G5 = 15
};

enum x86REG
{
    EAX = 0,
    ECX = 1,
    EDX = 2,
    EBX = 3,
    ESP = 4,
    EBP = 5,
    ESI = 6,
    EDI = 7
};

enum x64REG
{
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
    R8 = 0,
    R9 = 1,
    R10 = 2,
    R11 = 3,
    R12 = 4,
    R13 = 5,
    R14 = 6,
    R15 = 7
};
#endif // REGISTERS_H