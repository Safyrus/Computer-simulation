#pragma once
#include <string>
#include "Device.hpp"

class RAM : public Device
{
public:
    RAM(int l);
    ~RAM();
};