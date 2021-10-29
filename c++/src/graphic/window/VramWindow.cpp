#include "graphic/window/VramWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::VramWindow::VramWindow(std::shared_ptr<computer::VRAM> vram)
{
    this->vram = vram;
    windowName = "S257 Dynamic Recompiler - " + vram->getName() + " Window";
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::VramWindow::VramWindow(std::shared_ptr<computer::VRAM> vram, std::string windowName)
{
    this->vram = vram;
    this->windowName = windowName;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::VramWindow::VramWindow(std::shared_ptr<computer::VRAM> vram, std::string windowName, bool debug)
{
    this->vram = vram;
    this->windowName = windowName;
    this->debug = debug;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::VramWindow::~VramWindow()
{
}

void graphic::window::VramWindow::start()
{
    printDebug("Start");
    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());

    vramView = std::make_shared<view::VramView>(this->vram);
    vramView->setPos(0, 0);
    vramView->setSize(width, height);
}

void graphic::window::VramWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::window::VramWindow::loop()
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
                vramView->setPage(vramView->getPage() - 1);
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                vramView->setPage(vramView->getPage() + 1);
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
    vramView->draw(window);

    // Update the window
    window.display();
}
