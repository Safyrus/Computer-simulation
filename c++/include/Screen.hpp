#pragma once
#include <string>

#include <SFML/Graphics.hpp>

#include "Device.hpp"

class Screen : public Device
{
private:
    const int W = 256;
    const int H = 250;
    sf::RenderTexture RT;
    std::vector<sf::Vertex> pixMat;

    int8_t color[16];
public:
    Screen();
    ~Screen();

    void setData(int8_t d);
    int8_t getData();
    void setAdr(int a);

    void print(int x, int y);
    void display(sf::RenderWindow &window, int x, int y);
};