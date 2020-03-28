#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class CPU
{
private:
    int step;
    bool clk;
    bool pwr;
    bool load;

    static const int REGMAX = 16;
    static const int FLAG = 6;
    static const int RAN = 7;

    int adr;
    int adrBack;
    int data;
    int dataBack;
    int reg[REGMAX];

    int CU();
    int ALU(int8_t a, int8_t b, int8_t op, bool flag);

public:
    CPU();
    ~CPU();

    int getAdr();
    bool getLoad();
    int getData();
    void setData(int d);
    int getStep();
    bool getPwr();
    bool getClk();

    void setClk();
    void stp();
    void setRst();
    void setPwr();

    void print(int x, int y);
    void display(sf::RenderWindow &window, int x, int y);
};