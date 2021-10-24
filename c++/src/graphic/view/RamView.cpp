#include "graphic/view/RamView.hpp"
#include "utils/console.hpp"

#include <sstream>
#include <iomanip>

graphic::view::RamView::RamView(std::shared_ptr<computer::RAM> ram)
{
    this->ram = ram;
    page = 0;
    // setup font
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("RamView: Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);
}

graphic::view::RamView::~RamView()
{
}

void graphic::view::RamView::draw(sf::RenderWindow &window)
{
    // draw the background
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(w, h));
    window.draw(rect);

    // get ram data
    std::stringstream str;
    uint8_t max = 8;
    std::string strChar = "";
    str << "PAGE " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint16_t)page;
    uint16_t cntY = 0;
    uint16_t cntX = 0;
    for (int i = page * 256; i < (page * 256) + 256; i++)
    {
        if (((cntX + 2) * (4 * 4) >= w) || cntX == max || i == page * 256)
        {
            cntX = 0;
            cntY++;
            if (cntY != 1)
            {
                str << strChar;
            }
            strChar = "";
            str << "\n"
                << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << i << ": ";
        }
        cntX++;
        str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint16_t)ram->get(i) << " ";
        uint8_t chr = ram->get(i);
        if ((chr > 0x1F && chr < 0x7F) || chr > 0x9F)
        {
            strChar += (char)ram->get(i);
        }
        else
        {
            strChar += '.';
        }
    }
    str << strChar;

    // draw ram content
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(6);
    text.setString(str.str());
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(x, y);
    text.setPosition(pos);
    window.draw(text);
}

void graphic::view::RamView::setPage(uint8_t page)
{
    page %= ram->length()/256;
    this->page = page;
}

uint8_t graphic::view::RamView::getPage()
{
    return page;
}
