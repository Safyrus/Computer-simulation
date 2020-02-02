#include <sstream>
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#endif // _WIN32
#include "Keyboard.hpp"

Keyboard::Keyboard(int l) : Device(l)
{
    cursor = 0;
}

Keyboard::~Keyboard()
{
}

#ifdef _WIN32
char Keyboard::get_Key()
{
    if(!_kbhit())
        return 0;
    return getch();
}
#else
char Keyboard::get_Key()
{
}
#endif // _WIN32

void Keyboard::getKey()
{
    char key = get_Key();
    if(key != 0)
    {
        data[cursor] = key;
        if(cursor<len)
            cursor++;
    }
}

int8_t Keyboard::getData()
{
    cursor--;
    if(cursor<0)
    {
        cursor = 0;
        return 0;
    }
    int8_t c = data[0];
    for (int i = 0; i < len-1; i++)
    {
        data[i] = data[i+1];
    }
    data[len-1] = 0;
    return c;
}

void Keyboard::print(int x, int y)
{
    std::stringstream ss;
    ss << "\x1b[" << y << ";" << x << "H";
    std::cout << ss.str() << "|KEY:  ";
    for (int i = 0; i < len; i++)
    {
        if(cursor == i)
        {
            std::cout << "\x1b[30m\x1b[107m";
            std::cout << data[i];
            std::cout << "\x1b[0m" << " ";
        }else
        {
            std::cout << data[i] << " ";
        }
    }
}