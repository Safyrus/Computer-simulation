#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

#include "computer_old/CPU.hpp"
#include "computer_old/Device.hpp"

class Computer
{
private:
    unsigned int Hz;
    unsigned int cycleCount;
    bool pause;

    CPU *cpu;
    std::vector<Device*> devices;
    std::vector<int> adrDeviceStart;
    std::vector<int> adrDeviceEnd;

public:
    Computer();
    Computer(int hz);
    ~Computer();

    void setPwr();
    bool getPwr();
    void setPause();
    bool getPause();
    void setHz(unsigned int hz);
    void setCycle(unsigned int c);
    unsigned int getHz();
    unsigned int getCycle();

    void cycle();
    void halfCycle();
    void addDevice(Device *d, int adrStart, int adrEnd);
    void removeDevice(Device *d);

    void print(int x, int y);
    void display(sf::RenderWindow &window, int x, int y);
};