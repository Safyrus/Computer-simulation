#include "graphic/MenuView.hpp"
#include "utils/console.hpp"

#include <iostream>

graphic::MenuView::MenuView(std::shared_ptr<data::menu::Menu> menu)
{
    this->menu = menu;
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("MenuView: Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);
    x = 0;
    y = 0;
    w = 1;
    h = 1;
    scale = 1;
    released = false;
    pressed = false;
}

graphic::MenuView::~MenuView()
{
}

void graphic::MenuView::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void graphic::MenuView::setSize(int w, int h)
{
    if (w < 2)
        w = 2;
    if (h < 2)
        h = 2;
    this->w = w;
    this->h = h;
}

void graphic::MenuView::setScale(float s)
{
    if(s <= 0)
        s = 1;
    this->scale = s;
}

void graphic::MenuView::setMousePos(int x, int y)
{
    mx = x;
    my = y;
    //std::cout << x << "  " << y << std::endl;
}

void graphic::MenuView::setMousePressed(bool pressed)
{
    this->pressed = pressed;
    released = false;
    //std::cout << "pressed" << std::endl;
}

void graphic::MenuView::setMouseReleased(bool released)
{
    this->released = released;
    pressed = false;
    //std::cout << "released" << std::endl;
}

void graphic::MenuView::draw(sf::RenderWindow &window)
{
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color(0xff, 0xec, 0x80));
    rect.setSize(sf::Vector2f(w, h));

    int fontHeight = 6;
    int fontWidth = 4;
    sf::Text text;
    text.setFont(font);
    text.setPosition(x, y);
    text.setFillColor(sf::Color(0xff, 0xff, 0xff));
    text.setCharacterSize(fontHeight);

    std::string str;
    int nbChar = 0;
    int lastNbChar = 0;
    int x1 = x + (lastNbChar*fontWidth);
    int x2 = x + (nbChar*fontWidth);
    bool cursorOnMenu = false;
    int selectedIndex = 0;
    for (int i = 0; i < menu->size(); i++)
    {
        str += menu->getName(i);
        str += ' ';//0xDC;
        lastNbChar = nbChar;
        nbChar += menu->getName(i).length() + 1;
        if(!cursorOnMenu)
        {
            x1 = x + (lastNbChar*fontWidth);
            x2 = x + (nbChar*fontWidth);
            cursorOnMenu = (mx >= x1 && mx <= x2 && mx <= w && my >= y && my <= y+h);
            selectedIndex = i;
        }
    }
    text.setString(str);

    window.draw(rect);

    if(cursorOnMenu)
    {
        //std::cout << "mouseOnMenu: " << x1 << " " << x2-x1 << std::endl;
        rect.setPosition(x1, y);
        rect.setFillColor(sf::Color(0xff, 0x80, 0x00));
        rect.setSize(sf::Vector2f(x2-x1, h));
        if(pressed)
        {
            menu->select(menu->getName(selectedIndex));
            rect.setFillColor(sf::Color(0x60, 0x1b, 0x00));
        }
        window.draw(rect);
    }
    if(released)
    {
        if(cursorOnMenu && selectedIndex >= 0 && selectedIndex < menu->size())
        {
            menu->execute();
        }
        released = false;
    }

    window.draw(text);
}
