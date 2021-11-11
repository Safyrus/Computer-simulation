#include "graphic/window/VramWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::VramWindow::VramWindow(std::shared_ptr<computer::VRAM> vram)
{
    this->vram = vram;
    windowName = "S257 Dynamic Recompiler - " + vram->getName() + " Window";
    width = 286;
    height = 108;
    printDebug("Creation");
}

graphic::window::VramWindow::VramWindow(std::shared_ptr<computer::VRAM> vram, std::string windowName, bool debug)
{
    this->vram = vram;
    this->windowName = windowName;
    this->debug = debug;
    width = 286;
    height = 108;
    printDebug("Creation");
}

graphic::window::VramWindow::~VramWindow()
{
}

void graphic::window::VramWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    vramView = std::make_shared<view::VramView>(this->vram);
    vramView->setPos(0, 0);
    vramView->setSize(width, height);
}

void graphic::window::VramWindow::stop()
{
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::VramWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));
    windowTexture.draw(rect);

    // display the ram infos
    vramView->draw(windowTexture);

    // Update the window
    windowTexture.display();
}

void graphic::window::VramWindow::doEvent(sf::Event &event)
{
    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Left)
        {
            vramView->setPage(vramView->getPage() - 1);
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            vramView->setPage(vramView->getPage() + 1);
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