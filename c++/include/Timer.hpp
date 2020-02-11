#pragma once
#include "Device.hpp"

class Timer : public Device
{
private:

public:
    Timer();
    ~Timer();

    int8_t getData();
    int getData4();
    void setData(int8_t d);
};
