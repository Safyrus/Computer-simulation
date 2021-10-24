#include "graphic/window/RamWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::RamWindow::RamWindow(std::shared_ptr<computer::RAM> ram)
{
    this->ram = ram;
    windowName = "S257 Dynamic Recompiler - " + ram->getName() + " Window";
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::RamWindow::RamWindow(std::shared_ptr<computer::RAM> ram, std::string windowName)
{
    this->ram = ram;
    this->windowName = windowName;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::RamWindow::RamWindow(std::shared_ptr<computer::RAM> ram, std::string windowName, bool debug)
{
    this->ram = ram;
    this->windowName = windowName;
    this->debug = debug;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::RamWindow::~RamWindow()
{
}

void graphic::window::RamWindow::start()
{
    printDebug("Start");
    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());

    ramView = std::make_shared<view::RamView>(this->ram);
    ramView->setPos(0, 0);
    ramView->setSize(width, height);
}

void graphic::window::RamWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::window::RamWindow::loop()
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
                ramView->setPage(ramView->getPage() - 1);
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                ramView->setPage(ramView->getPage() + 1);
            }
            break;
        default:
            break;
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));
    window.draw(rect);

    // display the ram infos
    ramView->draw(window);

    // Update the window
    window.display();
}
