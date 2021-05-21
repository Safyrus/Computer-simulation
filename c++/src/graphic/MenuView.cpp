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
}

graphic::MenuView::~MenuView()
{
}

void graphic::MenuView::draw(sf::RenderWindow &window)
{
    sf::Color textColor = sf::Color(0x56, 0x6c, 0x86);
    sf::Color selectTextColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color backColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color selectColor = sf::Color(0x94, 0xb0, 0xc2);
    sf::Color activeColor = sf::Color(0x56, 0x6c, 0x86);

    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(backColor);
    rect.setSize(sf::Vector2f(w, h));

    int fontHeight = 6;
    int fontWidth = 4;
    sf::Text text;
    text.setFont(font);
    text.setPosition(x, y);
    text.setFillColor(textColor);
    text.setCharacterSize(fontHeight);

    std::string str;
    std::string strSelect;
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
            cursorOnMenu = (mx >= x && mx >= x1-(fontWidth/2) && mx < x2-(fontWidth/2) && mx < w && my >= y && my < y+h);
            selectedIndex = i;
            strSelect = menu->getName(i);
        }
    }
    text.setString(str);

    window.draw(rect);
    window.draw(text);

    if(cursorOnMenu)
    {
        //std::cout << "mouseOnMenu: " << x1 << " " << x2-x1 << std::endl;
        int posX = std::max(x, x1-(fontWidth/2));
        int sizeW = (posX == x)?x2-x1-(fontWidth/2):x2-x1;
        rect.setPosition(posX, y);
        rect.setFillColor(selectColor);
        rect.setSize(sf::Vector2f(sizeW, h));

        text.setPosition(x1, y);
        text.setFillColor(selectTextColor);
        text.setString(strSelect);
        if(pressed)
        {
            menu->select(menu->getName(selectedIndex));
            rect.setFillColor(activeColor);
        }
        window.draw(rect);
        window.draw(text);
    }
    if(released)
    {
        if(cursorOnMenu && selectedIndex >= 0 && selectedIndex < menu->size())
        {
            menu->execute();
        }
        released = false;
    }
}
