#include <chrono>
#include <iostream>
#include "global.hpp"
#include "computer_old/Timer.hpp"

Timer::Timer()
{
    len = 1;
    adr = 0;
}

Timer::~Timer()
{
}

void Timer::setData(int8_t d)
{
    //do nothing
    if(false)
        std::cout << d;
}

int Timer::getData4()
{
    return 0;
}

int8_t Timer::getData()
{
    std::chrono::milliseconds ms;
    switch (adr)
    {
    case 0:
        ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
        );
        return (int8_t)((ms.count()*255)/1000);
        break;
    default:
        return 0;
        break;
    }
}

void Timer::reset()
{
    if(print_debug)
        std::cout << "reset timer\n";
    adr = 0;
    if(print_debug)
        std::cout << "end reset timer\n";
}