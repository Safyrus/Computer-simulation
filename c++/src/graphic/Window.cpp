#include "graphic/Window.hpp"

#include <iostream>

#include "utils/console.hpp"

graphic::Window::Window()
{
    windowName = "noName";
    width = 10;
    height = 10;
}

graphic::Window::Window(std::string windowName)
{
    this->windowName = windowName;
    width = 10;
    height = 10;
}

graphic::Window::Window(std::string windowName, bool debug)
{
    this->windowName = windowName;
    this->debug = debug;
    width = 10;
    height = 10;
}

graphic::Window::~Window()
{
}

void graphic::Window::addSubWindow(std::shared_ptr<graphic::Window> subWindow)
{
    subWindow->run = true;
    subWindow->debug = debug;
    subWindow->start();
    subWindows.push_back(subWindow);
}

std::vector<std::shared_ptr<graphic::Window>> graphic::Window::findSubWinByName(std::string name)
{
    std::vector<std::shared_ptr<graphic::Window>> vector;
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        if (subWindows[i]->windowName.compare(name) == 0)
        {
            vector.push_back(subWindows[i]);
        }
    }
    return vector;
}

void graphic::Window::printDebug(std::string msg)
{
    if(debug)
    {
        std::string printPrefix = ansi(BLUE_FG) + "[" + windowName + "] " + ansi(RESET);
        std::cout << printPrefix << msg << std::endl;
    }
}

void graphic::Window::printError(std::string msg)
{
    std::string printPrefix = ansi(RED_FG) + "/!\\ ERROR /!\\ [" + windowName + "] " + ansi(RESET);
    std::cout << printPrefix << msg << std::endl;
}

sf::View graphic::Window::fixRatioCenterView()
{
    float scrW = window.getSize().x;
    float scrH = window.getSize().y;
    float ratio = (float)width / (float)height;
    float screenRatio = scrW / scrH;
    float xoff = 0;
    float yoff = 0;
    float w = 1;
    float h = 1;

    if (ratio < screenRatio) // width larger
    {
        w = ratio/screenRatio;
        xoff = (1-w)/2;
    }
    else // height larger
    {
        h = screenRatio/ratio;
        yoff = (1-h)/2;
    }

    sf::View view;
    view.setSize(width, height);
    view.setCenter(width / 2, height / 2);
    view.setViewport(sf::FloatRect(xoff, yoff, w, h));
    return view;
}

void graphic::Window::display()
{
    run = true;

    // starting
    start();
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::Window> subW = subWindows[i];
        subW->start();
    }

    // while running or window is open
    while (run)
    {
        loop();
        for (unsigned int i = 0; i < subWindows.size(); i++)
        {
            std::shared_ptr<graphic::Window> subW = subWindows[i];
            if (subW->run)
            {
                subW->loop();
            }
            else
            {
                subW->stop();
                subWindows.erase(subWindows.begin() + i);
            }
        }
    }

    // stoping
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::Window> subW = subWindows[i];
        subW->stop();
    }
    stop();

    run = false;
}
