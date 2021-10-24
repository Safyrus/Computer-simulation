#include "graphic/window/RomWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::RomWindow::RomWindow(std::shared_ptr<computer::ROM> rom)
{
    this->rom = rom;
    windowName = "S257 Dynamic Recompiler - " + rom->getName() + " Window";
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::RomWindow::RomWindow(std::shared_ptr<computer::ROM> rom, std::string windowName)
{
    this->rom = rom;
    this->windowName = windowName;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::RomWindow::RomWindow(std::shared_ptr<computer::ROM> rom, std::string windowName, bool debug)
{
    this->rom = rom;
    this->windowName = windowName;
    this->debug = debug;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::RomWindow::~RomWindow()
{
}

void graphic::window::RomWindow::start()
{
    printDebug("Start");
    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());

    romView = std::make_shared<view::RomView>(this->rom);
    romView->setPos(0, 0);
    romView->setSize(width, height);
}

void graphic::window::RomWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::window::RomWindow::loop()
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
        case sf::Event::KeyPressed:
            printDebug("Key " + std::to_string(event.key.code) + " Pressed");
            if (event.key.code == sf::Keyboard::Left)
            {
                romView->setPage(romView->getPage() - 1);
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                romView->setPage(romView->getPage() + 1);
            }
            break;
        default:
            break;
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));
    window.draw(rect);

    // display the rom infos
    romView->draw(window);

    // Update the window
    window.display();
}
