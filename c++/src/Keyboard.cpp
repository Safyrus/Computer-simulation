#include "Keyboard.hpp"
#ifdef _WIN32
#include <conio.h>
#else
#endif // _WIN32

Keyboard::Keyboard(int l) : Device(l)
{
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
        data[adr] = key;
        if(adr<len)
            adr++;
    }
}

int8_t Keyboard::getData()
{
    adr--;
    if(adr<0)
    {
        adr = 0;
        return 0;
    }
    return data[adr];
}