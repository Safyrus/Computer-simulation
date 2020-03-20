#pragma once
#include <string>
#include "computer/Device.hpp"

class DISK : public Device
{
public:
    DISK(int l);
    ~DISK();

    bool load(const char *f);
    bool save(const char *f);

    void display(sf::RenderWindow &window, int x, int y);
};