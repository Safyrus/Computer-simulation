#include "graphic/window/HwStatesWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::HwStatesWindow::HwStatesWindow(std::shared_ptr<computer::HardwareStates> hwstats)
{
    this->hwstats = hwstats;
    windowName = "S257 Dynamic Recompiler - Hardware States Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::HwStatesWindow::HwStatesWindow(std::shared_ptr<computer::HardwareStates> hwstats, std::string windowName)
{
    this->hwstats = hwstats;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::HwStatesWindow::HwStatesWindow(std::shared_ptr<computer::HardwareStates> hwstats, std::string windowName, bool debug)
{
    this->debug = debug;
    this->hwstats = hwstats;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::HwStatesWindow::~HwStatesWindow()
{
}

void graphic::window::HwStatesWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::window::HwStatesWindow::start()
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
    openTexture(board, "data/img/hwstates_board.png");
    openTexture(led_green_on, "data/img/hwstats_led_green_on.png");
    openTexture(led_red_on, "data/img/hwstats_led_red_on.png");

    // setup timer text
    timerTxt.setFont(font);
    timerTxt.setCharacterSize(6);
    timerTxt.setString("");
    timerTxt.setFillColor(sf::Color::White);
    timerTxt.setPosition(sf::Vector2f(61, 32));

    // setup buzzer text
    buzzerTxt.setFont(font);
    buzzerTxt.setCharacterSize(6);
    buzzerTxt.setString("");
    buzzerTxt.setFillColor(sf::Color::White);
    buzzerTxt.setPosition(sf::Vector2f(98, 36));

    // setup background
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());
}

void graphic::window::HwStatesWindow::stop()
{
    hwstats.reset();
    window.close();
    printDebug("Stop");
}

void graphic::window::HwStatesWindow::loop()
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
    if (hwstats->getPwr())
    {
        pwrLightSprite.setTexture(led_green_on);
        pwrLightSprite.setPosition(sf::Vector2f(17, 17));
        window.draw(pwrLightSprite);
    }

    // draw connected devices led
    uint8_t connected = hwstats->get(0);
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((connected & 0x80))
        {
            sf::Sprite ledSprite;
            ledSprite.setTexture(led_red_on);
            ledSprite.setPosition(sf::Vector2f(25 + (6 * i), 99));
            window.draw(ledSprite);
        }
        connected = connected << 1;
    }

    // draw timer text
    uint8_t val = hwstats->get(1);
    std::stringstream timerStr;
    timerStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)val;
    timerTxt.setString(timerStr.str());
    window.draw(timerTxt);

    // draw buzzer text
    val = hwstats->get(2);
    std::stringstream buzerStr;
    buzerStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)val;
    buzzerTxt.setString(buzerStr.str());
    window.draw(buzzerTxt);

    // Update the window
    window.display();
}
