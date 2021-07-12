#include "graphic/IOControllerWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::IOControllerWindow::IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl)
{
    this->ioCtrl = ioCtrl;
    windowName = "S257 Dynamic Recompiler - " + ioCtrl->getName() + " Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::IOControllerWindow::IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl, std::string windowName)
{
    this->ioCtrl = ioCtrl;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::IOControllerWindow::IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl, std::string windowName, bool debug)
{
    this->ioCtrl = ioCtrl;
    this->windowName = windowName;
    this->debug = debug;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::IOControllerWindow::~IOControllerWindow()
{
}

void graphic::IOControllerWindow::start()
{
    printDebug("Start");
    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());

    view = std::make_shared<IOControllerView>(this->ioCtrl);
    view->setPos(0, 0);
    view->setSize(width, height);
}

void graphic::IOControllerWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::IOControllerWindow::loop()
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

    // display the ioCtr infos
    view->draw(window);

    // Update the window
    window.display();
}
