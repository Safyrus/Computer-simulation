#include "graphic/window/CPUWindow.hpp"

#include "data/menu/MenuActionTest.hpp"

#include <sstream>
#include <iomanip>


graphic::window::CPUWindow::CPUWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName, bool debug)
{
    this->cpu = cpu;
    this->windowName = windowName;
    this->debug = debug;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::CPUWindow::~CPUWindow()
{
}

void graphic::window::CPUWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::window::CPUWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    // load font
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    // load texture
    openTexture(board, "data/img/cpu_board.png");

    // setup text
    text.setFont(font);
    text.setCharacterSize(6);
    text.setString("");
    text.setFillColor(sf::Color::White);

    // setup background
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));
}

void graphic::window::CPUWindow::stop()
{
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::CPUWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));

    windowTexture.draw(rect);

    // draw board
    sf::Sprite boardSprite;
    boardSprite.setTexture(board);
    boardSprite.setPosition(sf::Vector2f(0, 0));
    windowTexture.draw(boardSprite);

    // draw hz
    std::stringstream txt;
    txt << cpu->hz;
    text.setPosition(sf::Vector2f(16, 18));
    text.setString(txt.str());
    windowTexture.draw(text);

    // draw cycle
    txt.str("");
    txt << cpu->cycle;
    text.setPosition(sf::Vector2f(16, 42));
    text.setString(txt.str());
    windowTexture.draw(text);

    // draw adr
    txt.str("");
    txt << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << cpu->pc;
    text.setPosition(sf::Vector2f(16, 66));
    text.setString(txt.str());
    windowTexture.draw(text);

    // draw basic registers
    for (unsigned int i = 0; i < 8; i++)
    {
        txt.str("");
        txt << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)(cpu->reg[i] & 0xFF);
        text.setPosition(sf::Vector2f(48 + (i * 9), 66));
        text.setString(txt.str());
        windowTexture.draw(text);
    }

    // draw jump registers
    for (unsigned int i = 0; i < 2; i++)
    {
        txt.str("");
        txt << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)(cpu->reg[i + 8] & 0xFF);
        text.setPosition(sf::Vector2f(16 + (i * 11), 84));
        text.setString(txt.str());
        windowTexture.draw(text);
    }

    // draw general registers
    for (unsigned int i = 0; i < 6; i++)
    {
        txt.str("");
        txt << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)(cpu->reg[i + 10] & 0xFF);
        text.setPosition(sf::Vector2f(48 + (i * 11), 84));
        text.setString(txt.str());
        windowTexture.draw(text);
    }

    // Update the window
    windowTexture.display();
}
