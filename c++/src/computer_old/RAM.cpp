#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include "computer_old/RAM.hpp"
#include "global.hpp"

RAM::RAM(int l) : Device(l)
{
}

RAM::~RAM()
{
}

void RAM::display(sf::RenderWindow &window, int x, int y)
{
    float charSizeX = 8;
    float charSizeY = 10;

    //draw background rect
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(charSizeX*48, charSizeY*16));
    rect.setFillColor(sf::Color(255, 0, 0));
    window.draw(rect);

    //init text
    sf::Text text;
    text.setCharacterSize(20);
    text.setScale(0.5, 0.5);
    text.setFont(baseFont);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(x, y);
    std::stringstream ss ;
    ss << std::hex;

    //get string to display
    int start = (adr/256)*256;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            int at = ((i*16)+j);
            ss << std::uppercase << std::setfill('0') << std::setw(2) << (int)(data[at + start]&0x000000ff) << " ";
        }
        ss << "\n";
    }

    //display text
    text.setString(ss.str());
    window.draw(text);

    //draw selection rect
    int adrX = adr%16;
    int adrY = adr%256/16;
    rect.setPosition(x + charSizeX*adrX*3, y + charSizeY*adrY);
    rect.setSize(sf::Vector2f(charSizeX*3, charSizeY));
    rect.setFillColor(sf::Color(255, 255, 255));
    window.draw(rect);

    // draw selection string
    text.setFillColor(sf::Color(0, 0, 0));
    text.setPosition(x + charSizeX*adrX*3, y + charSizeY*adrY);
    ss.str("");
    ss << std::uppercase << std::setfill('0') << std::setw(2) << (int)(data[adr]&0x000000ff) << " ";
    text.setString(ss.str());
    window.draw(text);
}
