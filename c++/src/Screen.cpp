#include <iostream>
#include <sstream>
#include "Screen.hpp"

Screen::Screen(int l) : Device(l)
{
}

Screen::~Screen()
{
}

void Screen::print(int x, int y)
{
    int resX = data[0];
    int resY = data[1];
    int mode = data[2];
    int count = 3;
    std::stringstream ss;
    ss << "\x1b[" << y << ";" << x << "H";
    std::cout << ss.str();

    switch (mode)
    {
    case 0:
        for (int i = 0; i < resX; i++)
        {
            for (int j = 0; j < resY; j++)
            {
                ss.clear();
                ss << "\x1b[" << y+j << ";" << x+i << "H";
                if(data[count] != 0)
                    ss << "\x1b[102m";
                else
                    ss << "\x1b[42m";
                std::cout << ss.str() << ' ';
                count++;
            }
        }
        break;
    }
    ss.clear();
    ss << "\x1b[0m";
    std::cout << ss.str();
}