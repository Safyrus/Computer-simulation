#include "graphic/window/BaseWindow.hpp"

graphic::window::BaseWindow::BaseWindow()
{
}

graphic::window::BaseWindow::~BaseWindow()
{
}

void graphic::window::BaseWindow::start()
{
    window.create(sf::VideoMode(640, 480), "Base window");
}

void graphic::window::BaseWindow::stop()
{
}

void graphic::window::BaseWindow::loop()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            window.close();
    }
    // Clear screen
    window.clear();

    // Update the window
    window.display();
}
