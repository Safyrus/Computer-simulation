#include <sstream>
#include <iostream>

#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <conio.h>
#else
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#endif // _WIN32
#include "Keyboard.hpp"
#include "global.hpp"

Keyboard::Keyboard(int l) : Device(l)
{
    cursor = 0;
    control = 0;
}

Keyboard::~Keyboard()
{
}

#ifdef _WIN32
int Keyboard::get_Key()
{
    if (!_kbhit())
        return 0;
    return getch();
}
#else
int Keyboard::get_Key()
{
    //terminal need to be in raw mode
    return fgetc(stdin);
}
#endif // _WIN32

void Keyboard::getKey()
{
    char key = get_Key();
    if (key != EOF && key != 0)
    {
        data[cursor] = key;
        if (cursor < len)
            cursor++;
    }
}

int8_t Keyboard::getData()
{
    if(adr==0)
    {
        cursor--;
        if (cursor < 0)
        {
            cursor = 0;
            return 0;
        }
        int8_t c = data[0];
        for (int i = 0; i < len - 1; i++)
        {
            data[i] = data[i + 1];
        }
        data[len - 1] = 0;
        return c;
    }else if (adr==1)
    {
        return control;
    }
    return 0;
    
}

void Keyboard::setControl(int8_t c)
{
    control = c;
}

int8_t Keyboard::getControl()
{
    return control;
}

void Keyboard::setData(int8_t d)
{
    if(adr==0)
    {
        setKey(d);
    }else if (adr==1)
    {
        control = d;
    }
}

void Keyboard::setKey(int c)
{
    char key = c;
    /*if(control&&0x02)
    {
        key += 128;
    }*/
    if (key != EOF && key != 0)
    {
        data[cursor] = key;
        if (cursor < len)
            cursor++;
    }
}

void Keyboard::print(int x, int y)
{
    std::stringstream ss;
    ss << "\x1b[" << y << ";" << x << "H";
    std::cout << ss.str() << "|KEY:  ";
    for (int i = 0; i < len; i++)
    {
        if (cursor == i)
        {
            std::cout << "\x1b[30m\x1b[107m";
            std::cout << (data[i]&0xff);
            std::cout << "\x1b[0m"
            << " ";
        }
        else
        {
            std::cout << (data[i]&0xff) << " ";
        }
    }
}

void Keyboard::display(sf::RenderWindow &window, int x, int y)
{
    int charSize = 10;
    sf::Text text;
    text.setCharacterSize(charSize);
    text.setFont(baseFont);
    text.setPosition(x, y);
    std::stringstream ss;

    ss << "KEYBOARD BUFFER:\n";
    for (int i = 0; i < len; i++)
    {
        ss << (char)(data[i]&0xff) << " ";
    }
    ss << "\nCTRL: " << std::hex << (int)control;
    
    text.setString(ss.str());
    window.draw(text);
}
