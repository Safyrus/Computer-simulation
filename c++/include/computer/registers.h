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
    EBX = 3
};

#endif // REGISTERS_H