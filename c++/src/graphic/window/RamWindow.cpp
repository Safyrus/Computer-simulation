#include "graphic/window/RamWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::RamWindow::RamWindow(std::shared_ptr<computer::RAM> ram)
{
    this->ram = ram;
    windowName = "S257 Dynamic Recompiler - " + ram->getName() + " Window";
    width = 286;
    height = 108;
    printDebug("Creation");
}

graphic::window::RamWindow::RamWindow(std::shared_ptr<computer::RAM> ram, std::string windowName, bool debug)
{
    this->ram = ram;
    this->windowName = windowName;
    this->debug = debug;
    width = 286;
    height = 108;
    printDebug("Creation");
}

graphic::window::RamWindow::~RamWindow()
{
}

void graphic::window::RamWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    ramView = std::make_shared<view::RamView>(this->ram);
    ramView->setPos(0, 0);
    ramView->setSize(width, height);
}

void graphic::window::RamWindow::stop()
{
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::RamWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));
    windowTexture.draw(rect);

    // display the ram infos
    ramView->draw(windowTexture);

    // Update the window
    windowTexture.display();
}

void graphic::window::RamWindow::doEvent(sf::Event &event)
{
    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Left)
        {
            ramView->setPage(ramView->getPage() - 1);
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            ramView->setPage(ramView->getPage() + 1);
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