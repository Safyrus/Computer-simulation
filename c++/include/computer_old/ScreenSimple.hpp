#pragma once
#include <string>

#include <SFML/Graphics.hpp>

#include "computer_old/Device.hpp"

class ScreenSimple : public Device
{
private:
    const int W = 128;
    const int H = 128;
    sf::RenderTexture RT;
    std::vector<sf::Vertex> pixMat;

    sf::Color color[16];
public:
    ScreenSimple();
    ~ScreenSimple();

    void setData(int8_t d);
    void reset();

    void display(sf::RenderWindow &window, int x, int y);
};