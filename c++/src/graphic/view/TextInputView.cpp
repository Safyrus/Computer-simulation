#include "graphic/view/TextInputView.hpp"

#include "utils/console.hpp"

graphic::view::TextInputView::TextInputView()
{
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("MenuView: Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);
    input = "";
}

graphic::view::TextInputView::~TextInputView()
{
}

void graphic::view::TextInputView::draw(sf::RenderTexture &window)
{
    sf::Color textColor = sf::Color(0x1a, 0x1c, 0x2c);
    sf::Color backColor = sf::Color(0x94, 0xb0, 0xc2);
    sf::Color activeColor = sf::Color(0xf4, 0xf4, 0xf4);
    sf::Color outlineColor = sf::Color(0x1a, 0x1c, 0x2c);

    int width = w * scale;
    int height = h * scale;

    // draw background
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    if (active)
        rect.setFillColor(activeColor);
    else
        rect.setFillColor(backColor);
    rect.setSize(sf::Vector2f(width, height));
    rect.setOutlineColor(outlineColor);
    rect.setOutlineThickness(1);
    window.draw(rect);

    // setup text
    int fontHeight = 6 * scale;
    int fontWidth = 4 * scale;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(6);
    text.setPosition(x+1, y);
    text.setFillColor(textColor);
    text.setCharacterSize(fontHeight);

    // cut string if necessary
    std::string subStr = input;
    if ((int)(input.size() * fontWidth) > width)
    {
        int nbChar = width / fontWidth;
        subStr = subStr.substr(subStr.size()-nbChar);
    }
    text.setString(subStr);

    // draw text
    window.draw(text);
}

std::string graphic::view::TextInputView::getInput()
{
    return input;
}

void graphic::view::TextInputView::inputChar(char c)
{
    if (active)
    {
        if (c == 0x08)
        {
            if (!input.empty())
                input.pop_back();
        }
        else if (c < 0x20 || c == 0x7F)
        {
            return;
        }
        else
        {
            if(input.size() < limit)
                input += c;
        }
    }
}

void graphic::view::TextInputView::setActive(bool active)
{
    this->active = active;
}

bool graphic::view::TextInputView::isActive()
{
    return active;
}

void graphic::view::TextInputView::setLimit(unsigned int limit)
{
    this->limit = limit;
}
