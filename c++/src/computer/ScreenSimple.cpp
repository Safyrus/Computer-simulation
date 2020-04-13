#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "computer/ScreenSimple.hpp"
#include "global.hpp"

ScreenSimple::ScreenSimple()
{
    len = (H*W)/2;
    adr = 0;
    data = new int8_t[len];
    for (int i = 0; i < len; i++)
    {
        data[i] = 0;
    }

    color[0] = sf::Color(0, 0, 0);
    color[1] = sf::Color(96, 27, 0);
    color[2] = sf::Color(14, 96, 0);
    color[3] = sf::Color(0, 0, 96);
    color[4] = sf::Color(128, 0, 128);
    color[5] = sf::Color(200, 191, 255);
    color[6] = sf::Color(255, 128, 0);
    color[7] = sf::Color(229, 255, 191);
    color[8] = sf::Color(82, 82, 102);
    color[9] = sf::Color(255, 0, 0);
    color[10] = sf::Color(0, 255, 0);
    color[11] = sf::Color(0, 0, 255);
    color[12] = sf::Color(255, 191, 237);
    color[13] = sf::Color(191, 255, 255);
    color[14] = sf::Color(255, 236, 128);
    color[15] = sf::Color(255, 255, 255);

    RT.create(W, H);
    pixMat.resize(W * H);
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            pixMat[(i * W) + j] = sf::Vertex(sf::Vector2f(j + .5f, i + .5f), color[(j*16/W)]);
}

ScreenSimple::~ScreenSimple()
{
}

void ScreenSimple::setData(int8_t d)
{
    data[adr] = d;
    int8_t col1 = ((d & 0xf0) >> 4);
    int8_t col2 = (d & 0xf);
    int i = adr / (H/2);
    int j = (adr % (W/2))*2;
    pixMat[(i * W) + j] = sf::Vertex(sf::Vector2f(j + .5f, i + .5f), color[col1]);
    pixMat[(i * W) + j + 1] = sf::Vertex(sf::Vector2f(j + 1 + .5f, i + .5f), color[col2]);
}

void ScreenSimple::display(sf::RenderWindow &window, int x, int y)
{
    //Draws the pixel matrix
    RT.draw(pixMat.data(), H * W, sf::Points);

    //And finally draws the RenderTexture to the RenderWindow
    RT.display();
    sf::Sprite spr(RT.getTexture());
    spr.setPosition((float)x, (float)y);
    spr.setScale(2, 2);
    window.draw(spr);
}

void ScreenSimple::reset()
{
    adr = 0;
    for (int i = 0; i < len; i++)
    {
        data[i] = 0;
    }
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            pixMat[(i * W) + j] = sf::Vertex(sf::Vector2f(j + .5f, i + .5f), color[0]);
}