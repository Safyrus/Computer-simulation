#pragma once
#include <vector>
#include "CPU.hpp"
#include "Device.hpp"

class Computer
{
private:
    unsigned int Hz;
    unsigned int cycleCount;

    CPU *cpu;
    std::vector<Device*> devices;
    std::vector<int> adrDeviceStart;
    std::vector<int> adrDeviceEnd;

public:
    Computer();
    Computer(int hz);
    ~Computer();
    void cycle();
    void setPwr();
    bool getPwr();
    void setHz(unsigned int hz);
    void setCycle(unsigned int c);
    unsigned int getHz();
    unsigned int getCycle();

    void addDevice(Device *d, int adrStart, int adrEnd);
    void removeDevice(Device *d);

    void print(int x, int y);
};