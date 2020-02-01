#pragma once
#include <vector>
#include "CPU.hpp"
#include "Device.hpp"

class Computer
{
private:
    CPU *cpu;
    std::vector<Device*> devices;
    std::vector<int> adrDeviceStart;
    std::vector<int> adrDeviceEnd;

public:
    Computer();
    ~Computer();
    void cycle();
    void setPwr();
    bool getPwr();

    void addDevice(Device *d, int adrStart, int adrEnd);
    void removeDevice(Device *d);
};