#include "graphic/MenuView.hpp"
#include "utils/console.hpp"

#include <iostream>

graphic::MenuView::MenuView(std::shared_ptr<data::menu::Menu> menu)
{
    this->menu = menu;
    scale = 1;
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("MenuView: Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);
    vertical = false;
}

graphic::MenuView::MenuView(std::shared_ptr<data::menu::Menu> menu, bool vertical)
{
    this->menu = menu;
    scale = 1;
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("MenuView: Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);
    this->vertical = vertical;
}

graphic::MenuView::~MenuView()
{
}

void graphic::MenuView::drawHorinzontal(sf::RenderWindow &window)
{
    sf::Color textColor = sf::Color(0x56, 0x6c, 0x86);
    sf::Color selectTextColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color backColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color selectColor = sf::Color(0x94, 0xb0, 0xc2);
    sf::Color activeColor = sf::Color(0x56, 0x6c, 0x86);

    int width = w * scale;
    int height = h * scale;
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(backColor);
    rect.setSize(sf::Vector2f(width, height));

    int fontHeight = 6 * scale;
    int fontWidth = 4 * scale;
    sf::Text text;
    text.setFont(font);
    text.setPosition(x, y);
    text.setFillColor(textColor);
    text.setCharacterSize(fontHeight);

    std::string str;
    std::string strSelect;
    int nbChar = 0;
    int lastNbChar = 0;
    int x1 = x + (lastNbChar * fontWidth);
    int x2 = x + (nbChar * fontWidth);
    bool cursorOnMenu = false;
    int selectedIndex = 0;
    for (int i = 0; i < menu->size(); i++)
    {
        str += menu->getName(i);
        str += ' ';
        lastNbChar = nbChar;
        nbChar += menu->getName(i).length() + 1;
        if (!cursorOnMenu)
        {
            x1 = x + (lastNbChar * fontWidth);
            x2 = x + (nbChar * fontWidth);
            cursorOnMenu = (mx >= x && mx >= x1 - (fontWidth / 2) && mx < x2 - (fontWidth / 2) && mx < width && my >= y && my < y + height);
            selectedIndex = i;
            strSelect = menu->getName(i);
        }
    }
    text.setString(str);

    window.draw(rect);
    window.draw(text);

    if (cursorOnMenu)
    {
        int posX = std::max(x, x1 - (fontWidth / 2));
        int sizeW = (posX == x) ? x2 - x1 - (fontWidth / 2) : x2 - x1;
        rect.setPosition(posX, y);
        rect.setFillColor(selectColor);
        rect.setSize(sf::Vector2f(sizeW, height));

        text.setPosition(x1, y);
        text.setFillColor(selectTextColor);
        text.setString(strSelect);
        if (pressed)
        {
            menu->select(menu->getName(selectedIndex));
            rect.setFillColor(activeColor);
        }
        window.draw(rect);
        window.draw(text);
    }

    if (subMenu != nullptr)
    {
        subMenu->draw(window);
    }

    if (released)
    {
        if (cursorOnMenu && selectedIndex >= 0 && selectedIndex < menu->size())
        {
            std::shared_ptr<data::menu::Menu> m = std::dynamic_pointer_cast<data::menu::Menu>(menu->getAction(selectedIndex));
            if (m != nullptr)
            {
                int posX = std::max(x, x1 - (fontWidth / 2));
                subMenu = std::make_shared<MenuView>(m, true);
                subMenu->setScale(scale);
                subMenu->setPos(posX, y + fontHeight);
                subMenu->setSize(window.getSize().x - posX, window.getSize().y - (y + fontHeight));
            }
            else
            {
                menu->execute();
                subMenu = nullptr;
            }
        }
        else
        {
            if (subMenu != nullptr && subMenu->subMenu == nullptr)
            {
                subMenu = nullptr;
            }
        }
        released = false;
    }
}

void graphic::MenuView::drawVertical(sf::RenderWindow &window)
{
    sf::Color textColor = sf::Color(0x56, 0x6c, 0x86);
    sf::Color selectTextColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color backColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color selectColor = sf::Color(0x94, 0xb0, 0xc2);
    sf::Color activeColor = sf::Color(0x56, 0x6c, 0x86);

    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(backColor);

    int fontHeight = 6 * scale;
    int fontWidth = 4 * scale;
    sf::Text text;
    text.setFont(font);
    text.setPosition(x + (fontWidth / 2), y);
    text.setFillColor(textColor);
    text.setCharacterSize(fontHeight);

    std::string str;
    int largestStr = 0;
    for (int i = 0; i < menu->size(); i++)
    {
        str += menu->getName(i) + "\n";
        if (menu->getName(i).size() > (uint32_t)largestStr)
        {
            largestStr = menu->getName(i).size();
        }
    }

    rect.setSize(sf::Vector2f(largestStr * fontWidth + fontWidth, menu->size() * fontHeight));
    text.setString(str);
    window.draw(rect);
    window.draw(text);

    bool cursorOnMenu = false;
    int selectedIndex = 0;
    std::string strSelect;
    for (int i = 0; i < menu->size(); i++)
    {
        if (mx >= x && mx < x + (largestStr * fontWidth) + fontWidth && my >= y + (fontHeight * i) && my < y + (fontHeight * (i + 1)))
        {
            cursorOnMenu = true;
            selectedIndex = i;
            strSelect = menu->getName(i);
            break;
        }
    }
    if (cursorOnMenu)
    {
        rect.setPosition(x, y + (fontHeight * selectedIndex));
        rect.setFillColor(selectColor);
        rect.setSize(sf::Vector2f(largestStr * fontWidth + fontWidth, fontHeight));

        text.setPosition(x + (fontWidth / 2), y + (fontHeight * selectedIndex));
        text.setFillColor(selectTextColor);
        text.setString(strSelect);
        if (pressed)
        {
            menu->select(menu->getName(selectedIndex));
            rect.setFillColor(activeColor);
        }
        window.draw(rect);
        window.draw(text);
    }

    if (subMenu != nullptr)
    {
        subMenu->draw(window);
    }

    if (released)
    {
        if (cursorOnMenu && selectedIndex >= 0 && selectedIndex < menu->size())
        {
            std::shared_ptr<data::menu::Menu> m = std::dynamic_pointer_cast<data::menu::Menu>(menu->getAction(selectedIndex));
            if (m != nullptr)
            {
                subMenu = std::make_shared<MenuView>(m, true);
                subMenu->setScale(scale);
                subMenu->setPos(x + (largestStr * fontWidth) + fontWidth, y + (selectedIndex * fontHeight));
                subMenu->setSize(window.getSize().x, window.getSize().y);
            }
            else
            {
                menu->execute();
                subMenu = nullptr;
            }
        }
        else
        {
            if (subMenu != nullptr && subMenu->subMenu == nullptr)
            {
                subMenu = nullptr;
            }
        }
        released = false;
    }
}

void graphic::MenuView::setMousePos(int x, int y)
{
    View::setMousePos(x, y);
    if (subMenu != nullptr)
    {
        subMenu->setMousePos(x, y);
    }
}

void graphic::MenuView::setMousePressed(bool pressed)
{
    View::setMousePressed(pressed);
    if (subMenu != nullptr)
    {
        subMenu->setMousePressed(pressed);
    }
}

void graphic::MenuView::setMouseReleased(bool released)
{
    View::setMouseReleased(released);
    if (subMenu != nullptr)
    {
        subMenu->setMouseReleased(released);
    }
}

void graphic::MenuView::draw(sf::RenderWindow &window)
{
    if (vertical)
    {
        drawVertical(window);
    }
    else
    {
        drawHorinzontal(window);
    }
}

void graphic::MenuView::setScale(int scale)
{
    if (scale > 0)
    {
        this->scale = scale;
        const_cast<sf::Texture &>(font.getTexture(6 * scale)).setSmooth(false);
    }
}
