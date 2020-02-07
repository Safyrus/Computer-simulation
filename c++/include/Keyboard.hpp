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
    void setKey(int c);
    void print(int x, int y);
    void display(sf::RenderWindow &window, int x, int y);
};