#pragma once
#include <string>
#include "computer_old/Device.hpp"

class RAM : public Device
{
public:
    RAM(int l);
    ~RAM();

    void display(sf::RenderWindow &window, int x, int y);
};