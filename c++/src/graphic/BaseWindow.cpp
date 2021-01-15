#include "graphic/BaseWindow.hpp"

graphic::BaseWindow::BaseWindow()
{
}

graphic::BaseWindow::~BaseWindow()
{
}

void graphic::BaseWindow::start()
{
    window.create(sf::VideoMode(640, 480), "Base window");
}

void graphic::BaseWindow::stop()
{
}

void graphic::BaseWindow::loop()
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
