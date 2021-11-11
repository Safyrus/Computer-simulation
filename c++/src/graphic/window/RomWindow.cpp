#include "graphic/window/RomWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::RomWindow::RomWindow(std::shared_ptr<computer::ROM> rom)
{
    this->rom = rom;
    windowName = "S257 Dynamic Recompiler - " + rom->getName() + " Window";
    width = 286;
    height = 108;
    printDebug("Creation");
}

graphic::window::RomWindow::RomWindow(std::shared_ptr<computer::ROM> rom, std::string windowName, bool debug)
{
    this->rom = rom;
    this->windowName = windowName;
    this->debug = debug;
    width = 286;
    height = 108;
    printDebug("Creation");
}

graphic::window::RomWindow::~RomWindow()
{
}

void graphic::window::RomWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    romView = std::make_shared<view::RomView>(this->rom);
    romView->setPos(0, 0);
    romView->setSize(width, height);
}

void graphic::window::RomWindow::stop()
{
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::RomWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));
    windowTexture.draw(rect);

    // display the rom infos
    romView->draw(windowTexture);

    // Update the window
    windowTexture.display();
}

void graphic::window::RomWindow::doEvent(sf::Event &event)
{
    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Left)
        {
            romView->setPage(romView->getPage() - 1);
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            romView->setPage(romView->getPage() + 1);
        }
        else if (event.key.code == sf::Keyboard::Escape)
        {
            run = false;
        }
        break;
    default:
        break;
    }
}