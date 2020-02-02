#pragma once
#include <string>
#include "Device.hpp"

class Keyboard : public Device
{
private:
    char get_Key();
public:
    Keyboard(int l);
    ~Keyboard();

    int8_t getData();
    void getKey();
};