#pragma once
#include <string>

class CPU
{
private:
    int step;
    bool clk;
    bool pwr;
    bool load;

    static const int REGMAX = 8;
    static const int FLAG = 6;
    static const int RAN = 7;

    int adr;
    int adrBack;
    int data;
    int dataBack;
    int reg[REGMAX];

    int CU();
    int ALU(int a, int b, int op, bool flag);

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
};
