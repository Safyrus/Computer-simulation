#pragma once
#include <string>
#include "computer_old/Device.hpp"

class Keyboard : public Device
{
private:
    int cursor;
    int8_t control;
    int get_Key();
public:
    Keyboard(int l);
    ~Keyboard();

    int8_t getData();
    void setData(int8_t d);

    void getKey();
    void setKey(int c);
    int8_t getControl();
    void setControl(int8_t c);

    void print(int x, int y);
    void display(sf::RenderWindow &window, int x, int y);
};