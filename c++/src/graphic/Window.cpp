#include "graphic/Window.hpp"

#include <iostream>
#include <chrono>

#include "utils/console.hpp"

graphic::Window::Window()
{
    windowName = "noName";
    width = 10;
    height = 10;
    fps = 60;
}

graphic::Window::Window(std::string windowName)
{
    this->windowName = windowName;
    width = 10;
    height = 10;
    fps = 60;
}

graphic::Window::Window(std::string windowName, bool debug)
{
    this->windowName = windowName;
    this->debug = debug;
    width = 10;
    height = 10;
    fps = 60;
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

void graphic::Window::loopSubWindows()
{
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::Window> subW = subWindows[i];
        if (subW->run)
        {
            subW->loop();
            subW->loopSubWindows();
        }
        else
        {
            subW->stopSubWindows();
            subW->stop();
            subWindows.erase(subWindows.begin() + i);
        }
    }
}

void graphic::Window::stopSubWindows()
{
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::Window> subW = subWindows[i];
        subW->stopSubWindows();
        subW->stop();
    }
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
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        loop();
        loopSubWindows();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        int delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        int sleepFor = std::max(0, (1000/fps)-delta);
        sf::sleep(sf::milliseconds(sleepFor));
    }

    // stoping
    stopSubWindows();
    stop();

    run = false;
}

void graphic::Window::openSubWindow(std::string windowName)
{
    std::string msg = "methode 'openSubWindow' not define, cannot open " + windowName;
    printDebug(msg);
}
