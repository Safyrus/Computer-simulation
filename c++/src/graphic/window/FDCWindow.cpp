#include "graphic/window/FDCWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::FDCWindow::FDCWindow(std::shared_ptr<computer::FDC> fdc, std::string windowName, bool debug)
{
    this->debug = debug;
    this->fdc = fdc;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::FDCWindow::~FDCWindow()
{
}

void graphic::window::FDCWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::window::FDCWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    // load font
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    // load textures
    openTexture(board, "data/img/fdc_board.png");
    openTexture(led_green_on, "data/img/fdc_led_green_on.png");
    openTexture(led_red_on, "data/img/fdc_led_red_on.png");

    // setup text
    txt.setFont(font);
    txt.setCharacterSize(6);
    txt.setString("");
    txt.setFillColor(sf::Color(0xf4, 0xf4, 0xf4));

    // setup background
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));
}

void graphic::window::FDCWindow::stop()
{
    fdc.reset();
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::FDCWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));

    windowTexture.draw(rect);

    // draw board
    sf::Sprite boardSprite;
    boardSprite.setTexture(board);
    boardSprite.setPosition(sf::Vector2f(0, 0));
    windowTexture.draw(boardSprite);

    // draw pwr light
    sf::Sprite pwrLightSprite;
    if (fdc->getPwr())
    {
        pwrLightSprite.setTexture(led_green_on);
        pwrLightSprite.setPosition(sf::Vector2f(82, 10));
        windowTexture.draw(pwrLightSprite);
    }

    // draw memory access light
    sf::Sprite memLedSprite;
    bool memAccess = (fdc->get(0) & 0x80);
    if (memAccess)
    {
        memLedSprite.setTexture(led_red_on);
        memLedSprite.setPosition(sf::Vector2f(18, 106));
        windowTexture.draw(memLedSprite);
    }

    // draw texts
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t val = fdc->get(i);
        std::stringstream statesStr;
        statesStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)val;
        txt.setString(statesStr.str());
        txt.setPosition(sf::Vector2f(57, 40 + (i * 7)));
        windowTexture.draw(txt);
    }

    // Update the window
    windowTexture.display();
}

void graphic::window::FDCWindow::doEvent(sf::Event &event)
{
    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape)
        {
            run = false;
        }
        break;
    default:
        break;
    }
}