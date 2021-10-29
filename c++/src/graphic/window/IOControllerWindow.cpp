#include "graphic/window/IOControllerWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::IOControllerWindow::IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl)
{
    this->ioCtrl = ioCtrl;
    windowName = "S257 Dynamic Recompiler - " + ioCtrl->getName() + " Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::IOControllerWindow::IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl, std::string windowName)
{
    this->ioCtrl = ioCtrl;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::IOControllerWindow::IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl, std::string windowName, bool debug)
{
    this->ioCtrl = ioCtrl;
    this->windowName = windowName;
    this->debug = debug;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::IOControllerWindow::~IOControllerWindow()
{
}

void graphic::window::IOControllerWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::window::IOControllerWindow::start()
{
    printDebug("Start");
    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    // load font
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    // load textures
    openTexture(board, "data/img/io_board.png");
    openTexture(led_red_on, "data/img/io_led_red_on.png");

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

void graphic::window::IOControllerWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::window::IOControllerWindow::loop()
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

    if (ioCtrl->getPwr())
    {
        // draw input talk
        std::stringstream str;
        str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->get(33);
        txt.setString(str.str());
        txt.setPosition(sf::Vector2f(10, 41));
        window.draw(txt);

        // draw output talk
        str.str("");
        str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->get(34);
        txt.setString(str.str());
        txt.setPosition(sf::Vector2f(10, 85));
        window.draw(txt);

        // draw connected
        uint8_t connected = ioCtrl->get(32);
        for (uint8_t i = 0; i < 8; i++)
        {
            if (connected & 0x01)
            {
                // draw led
                sf::Sprite lightSprite;
                lightSprite.setTexture(led_red_on);
                lightSprite.setPosition(sf::Vector2f(27 + (i * 12), 64));
                window.draw(lightSprite);
            }
            connected = connected >> 1;
        }

        for (uint8_t i = 0; i < ioCtrl->DEVICES_SIZE; i++)
        {
            // draw in data
            str.str("");
            for (uint8_t j = 0; j < ioCtrl->IN_SIZE; j++)
            {
                str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->getInBuf(i, j) << "\n";
            }
            txt.setString(str.str());
            txt.setPosition(sf::Vector2f(25 + (i * 12), 12));
            window.draw(txt);

            // draw out data
            str.str("");
            for (uint8_t j = 0; j < ioCtrl->OUT_SIZE; j++)
            {
                str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)ioCtrl->getOutBuf(i, j) << "\n";
            }
            txt.setString(str.str());
            txt.setPosition(sf::Vector2f(25 + (i * 12), 72));
            window.draw(txt);
        }
    }

    // Update the window
    window.display();
}
