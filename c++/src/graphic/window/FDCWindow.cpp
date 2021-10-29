#include "graphic/window/FDCWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::FDCWindow::FDCWindow(std::shared_ptr<computer::FDC> fdc)
{
    this->fdc = fdc;
    windowName = "S257 Dynamic Recompiler - VPU Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::FDCWindow::FDCWindow(std::shared_ptr<computer::FDC> fdc, std::string windowName)
{
    this->fdc = fdc;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

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

    // create window
    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

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
    txt.setFillColor(sf::Color::White);

    // setup background
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());
}

void graphic::window::FDCWindow::stop()
{
    fdc.reset();
    window.close();
    printDebug("Stop");
}

void graphic::window::FDCWindow::loop()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            printDebug("Closing window");
            run = false;
            break;
        case sf::Event::Resized:
            printDebug("Resize");
            window.setView(fixRatioCenterView());
            break;
        default:
            break;
        }
    }

    // Clear screen
    window.clear(sf::Color(32, 32, 32));

    window.draw(rect);

    // draw board
    sf::Sprite boardSprite;
    boardSprite.setTexture(board);
    boardSprite.setPosition(sf::Vector2f(0, 0));
    window.draw(boardSprite);

    // draw pwr light
    sf::Sprite pwrLightSprite;
    if (fdc->getPwr())
    {
        pwrLightSprite.setTexture(led_green_on);
        pwrLightSprite.setPosition(sf::Vector2f(82, 10));
        window.draw(pwrLightSprite);
    }

    // draw memory access light
    sf::Sprite memLedSprite;
    bool memAccess = (fdc->get(0) & 0x80);
    if (memAccess)
    {
        memLedSprite.setTexture(led_red_on);
        memLedSprite.setPosition(sf::Vector2f(18, 106));
        window.draw(memLedSprite);
    }

    // draw texts
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t val = fdc->get(i);
        std::stringstream statesStr;
        statesStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)val;
        txt.setString(statesStr.str());
        txt.setPosition(sf::Vector2f(57, 40 + (i * 7)));
        window.draw(txt);
    }

    // Update the window
    window.display();
}
