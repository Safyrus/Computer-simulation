#include "graphic/view/View.hpp"
#include "utils/console.hpp"

graphic::view::View::View()
{
    printDebug("View Creation");
    x = 0;
    y = 0;
    w = 1;
    h = 1;
    scale = 1;
    released = false;
    pressed = false;
}

graphic::view::View::~View()
{
    printDebug("View Destruction");
}

void graphic::view::View::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void graphic::view::View::setSize(int w, int h)
{
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
    this->w = w;
    this->h = h;
}

void graphic::view::View::setScale(float s)
{
    if(s <= 0)
        s = 1;
    this->scale = s;
}

void graphic::view::View::setMousePos(int x, int y)
{
    mx = x;
    my = y;
}

void graphic::view::View::setMousePressed(bool pressed)
{
    this->pressed = pressed;
    released = false;
}

void graphic::view::View::setMouseReleased(bool released)
{
    this->released = released;
    pressed = false;
}

int graphic::view::View::getPosX()
{
    return x;
}

int graphic::view::View::getPosY()
{
    return y;
}

int graphic::view::View::getWidth()
{
    return w;
}

int graphic::view::View::getHeight()
{
    return h;
}

