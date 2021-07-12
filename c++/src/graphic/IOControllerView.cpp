#include "graphic/IOControllerView.hpp"
#include "utils/console.hpp"

#include <sstream>
#include <iomanip>

graphic::IOControllerView::IOControllerView(std::shared_ptr<computer::IOController> ioCtrl)
{
    this->ioCtrl = ioCtrl;
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("RomView: Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);
}

graphic::IOControllerView::~IOControllerView()
{
}

void graphic::IOControllerView::draw(sf::RenderWindow &window)
{
    // draw the background
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(w, h));
    window.draw(rect);

    // get data
    std::stringstream str;
    str << "PORTS:\n " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->get(32) << " ";
    str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->get(33) << " ";
    str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->get(34) << "\nBUF:\n";
    for (uint8_t i = 0; i < ioCtrl->DEVICES_SIZE; i++)
    {
        str << " I:";
        for (uint8_t j = 0; j < ioCtrl->IN_SIZE; j++)
        {
            str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->getInBuf(i, j) << " ";
        }
        str << "\n O:";
        for (uint8_t j = 0; j < ioCtrl->OUT_SIZE; j++)
        {
            str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->getOutBuf(i, j) << " ";
        }
        str << "\n";
    }

    // draw text
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(6);
    text.setString(str.str());
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(x, y);
    text.setPosition(pos);
    window.draw(text);
}
