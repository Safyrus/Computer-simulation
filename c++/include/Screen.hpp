#pragma once
#include <string>
#include "Device.hpp"

class Screen : public Device
{
public:
    Screen(int l);
    ~Screen();

    void print(int x, int y);
};