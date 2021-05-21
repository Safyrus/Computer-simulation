#include "graphic/View.hpp"
#include "utils/console.hpp"

graphic::View::View()
{
    x = 0;
    y = 0;
    w = 1;
    h = 1;
    scale = 1;
    released = false;
    pressed = false;
}

graphic::View::~View()
{
}

void graphic::View::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void graphic::View::setSize(int w, int h)
{
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
    this->w = w;
    this->h = h;
}

void graphic::View::setScale(float s)
{
    if(s <= 0)
        s = 1;
    this->scale = s;
}

void graphic::View::setMousePos(int x, int y)
{
    mx = x;
    my = y;
}

void graphic::View::setMousePressed(bool pressed)
{
    this->pressed = pressed;
    released = false;
}

void graphic::View::setMouseReleased(bool released)
{
    this->released = released;
    pressed = false;
}

