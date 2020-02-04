#pragma once
#include <string>
#include "Device.hpp"

class Keyboard : public Device
{
private:
    int cursor;
    int get_Key();
public:
    Keyboard(int l);
    ~Keyboard();

    int8_t getData();
    void getKey();
    void print(int x, int y);
};