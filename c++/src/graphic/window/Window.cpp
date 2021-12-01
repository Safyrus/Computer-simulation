#include "graphic/window/Window.hpp"

#include <iostream>
#include <chrono>

#include "utils/console.hpp"

graphic::window::Window::Window()
{
    windowName = "noName";
    width = 10;
    height = 10;
    fps = 60;
    doResize = false;
    mainWindow = false;
}

graphic::window::Window::Window(std::string windowName, bool debug, bool oneWindowMode)
{
    this->windowName = windowName;
    this->debug = debug;
    this->oneWindowMode = oneWindowMode;
    width = 10;
    height = 10;
    fps = 60;
    doResize = false;
    mainWindow = false;
}

graphic::window::Window::~Window()
{
    printDebug("Destruction");
}

void graphic::window::Window::addSubWindow(std::shared_ptr<graphic::window::Window> subWindow)
{
    subWindow->oneWindowMode = oneWindowMode;
    if (oneWindowMode)
    {
        doResize = true;
    }
    subWindow->run = true;
    subWindow->debug = debug;
    subWindow->start();
    subWindows.push_back(subWindow);
}

std::vector<std::shared_ptr<graphic::window::Window>> graphic::window::Window::findSubWinByName(std::string name)
{
    std::vector<std::shared_ptr<graphic::window::Window>> vector;
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        if (subWindows[i]->windowName.compare(name) == 0)
        {
            vector.push_back(subWindows[i]);
        }
    }
    return vector;
}

void graphic::window::Window::printDebug(std::string msg)
{
    if (debug)
    {
        std::string printPrefix = ansi(BLUE_FG) + "[" + windowName + "] " + ansi(RESET);
        std::cout << printPrefix << msg << std::endl;
    }
}

void graphic::window::Window::printError(std::string msg)
{
    std::string printPrefix = ansi(RED_FG) + "/!\\ ERROR /!\\ [" + windowName + "] " + ansi(RESET);
    std::cout << printPrefix << msg << std::endl;
}

sf::View graphic::window::Window::fixRatioCenterView()
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
        w = ratio / screenRatio;
        xoff = (1 - w) / 2;
    }
    else // height larger
    {
        h = screenRatio / ratio;
        yoff = (1 - h) / 2;
    }

    sf::View view;
    view.setSize(width, height);
    view.setCenter(width / 2, height / 2);
    view.setViewport(sf::FloatRect(xoff, yoff, w, h));
    return view;
}

void graphic::window::Window::loopSubWindows()
{
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::window::Window> subW = subWindows[i];
        if (subW->run)
        {
            if (subW->window.isOpen())
            {
                sf::Event event;
                while (subW->window.pollEvent(event))
                {
                    subW->doEvent(event);
                }
            }

            if (!subW->oneWindowMode)
            {
                subW->loop();
                subW->loopSubWindows();
            }
            else
            {
                if (subW->subWindows.empty())
                {
                    subW->loop();
                }
                else
                {
                    subW->loopFirstSubWindow();
                }
            }

            if (subW->window.isOpen())
            {
                subW->window.clear();
                const sf::Texture &texture = subW->windowTexture.getTexture();
                sf::Sprite sprite(texture);
                subW->window.draw(sprite);
                subW->window.display();
            }
        }
        else
        {
            subW->stopSubWindows();
            subW->stop();
            subWindows.erase(subWindows.begin() + i);
        }
    }
}

void graphic::window::Window::loopFirstSubWindow()
{
    if (!subWindows.empty())
    {
        std::shared_ptr<graphic::window::Window> subW = subWindows[0];
        if (subW->run)
        {
            if (subW->window.isOpen())
            {
                sf::Event event;
                while (subW->window.pollEvent(event))
                {
                    subW->doEvent(event);
                }
            }

            if (!subW->oneWindowMode)
            {
                subW->loop();
                subW->loopSubWindows();
            }
            else
            {
                if (subW->subWindows.empty())
                {
                    subW->loop();
                }
                else
                {
                    subW->loopFirstSubWindow();
                }
            }

            if (subW->window.isOpen())
            {
                const sf::Texture &texture = subW->windowTexture.getTexture();
                sf::Sprite sprite(texture);
                subW->window.draw(sprite);
                subW->window.display();
            }
        }
        else
        {
            subW->stopSubWindows();
            subW->stop();
            subWindows.erase(subWindows.begin());
            doResize = true;
        }
    }
}

void graphic::window::Window::stopSubWindows()
{
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::window::Window> subW = subWindows[i];
        subW->stopSubWindows();
        subW->stop();
        subWindows.erase(subWindows.begin() + i);
    }
}

void graphic::window::Window::display()
{
    run = true;
    mainWindow = true;

    int mainWidth = width;
    int mainHeight = height;

    if (oneWindowMode)
    {
        window.create(sf::VideoMode(512, 512), windowName);
        window.setFramerateLimit(60);
        window.setView(fixRatioCenterView());
    }

    // starting
    start();
    for (unsigned int i = 0; i < subWindows.size(); i++)
    {
        std::shared_ptr<graphic::window::Window> subW = subWindows[i];
        subW->start();
    }

    // while running or window is open
    while (run)
    {
        if (window.isOpen())
        {
            sf::Event event;
            if (!oneWindowMode)
            {
                while (window.pollEvent(event))
                {
                    doEvent(event);
                }
            }
            else
            {
                if (getDoResize())
                {
                    printInfo("Resize");
                    if (subWindows.empty())
                    {
                        width = mainWidth;
                        height = mainHeight;
                    }
                    else
                    {
                        width = getFirstSubWindowWidth();
                        height = getFirstSubWindowHeight();
                    }
                    window.setView(fixRatioCenterView());
                    setDoResize(false);
                }
                while (window.pollEvent(event))
                {
                    sf::Vector2i mPos;
                    switch (event.type)
                    {
                    case sf::Event::Resized:
                        printDebug("Resize");
                        if (subWindows.empty())
                        {
                            width = mainWidth;
                            height = mainHeight;
                        }
                        else
                        {
                            width = getFirstSubWindowWidth();
                            height = getFirstSubWindowHeight();
                        }
                        window.setView(fixRatioCenterView());
                        break;
                    case sf::Event::MouseMoved:
                        mPos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                        mousePos = window.mapPixelToCoords(mPos);
                        setFirstSubWindowMousePos(mousePos);
                        break;
                    case sf::Event::KeyPressed:
                        printDebug("Key " + std::to_string(event.key.code) + " Pressed");
                        // If F11 is pressed
                        if (event.key.code == sf::Keyboard::F11)
                        {
                            fullscreen = !fullscreen;
                            if (fullscreen)
                            {
                                unsigned int w = sf::VideoMode::getDesktopMode().width;
                                unsigned int h = sf::VideoMode::getDesktopMode().height;
                                window.create(sf::VideoMode(w, h), windowName, sf::Style::Fullscreen);
                            }
                            else
                            {
                                window.create(sf::VideoMode(512, 512), windowName, sf::Style::Default);
                            }
                            window.setView(fixRatioCenterView());
                        }
                        break;
                    default:
                        break;
                    }
                    doEventFirstSubWindow(event);
                }
            }
        }

        if (!oneWindowMode)
        {
            loop();
            loopSubWindows();
        }
        else
        {
            if (subWindows.empty())
            {
                loop();
            }
            else
            {
                loopFirstSubWindow();
            }
        }

        if (window.isOpen())
        {
            window.clear();
            if (oneWindowMode && !subWindows.empty())
            {
                const sf::Texture &texture = subWindows[0]->getWindowTexture();
                sf::Sprite sprite(texture);
                window.draw(sprite);
            }
            else
            {
                const sf::Texture &texture = windowTexture.getTexture();
                sf::Sprite sprite(texture);
                window.draw(sprite);
            }
            window.display();
        }
    }

    // stoping
    stopSubWindows();
    stop();

    run = false;
    mainWindow = false;
}

void graphic::window::Window::openSubWindow(std::string windowName)
{
    std::string msg = "methode 'openSubWindow' not define, cannot open " + windowName;
    printDebug(msg);
}

void graphic::window::Window::doEvent(sf::Event &event)
{
    sf::Vector2i mPos;
    switch (event.type)
    {
    case sf::Event::Closed:
        printDebug("Closing window");
        run = false;
        break;
    case sf::Event::Resized:
        if (window.isOpen())
        {
            printDebug("Resize");
            window.setView(fixRatioCenterView());
        }
        break;
    case sf::Event::MouseMoved:
        if (window.isOpen())
        {
            mPos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            mousePos = window.mapPixelToCoords(mPos);
        }
        break;
    case sf::Event::KeyPressed:
        printDebug("Key " + std::to_string(event.key.code) + " Pressed");
        // If F11 is pressed
        if (event.key.code == sf::Keyboard::F11 && !oneWindowMode)
        {
            fullscreen = !fullscreen;
            if (fullscreen)
            {
                unsigned int w = sf::VideoMode::getDesktopMode().width;
                unsigned int h = sf::VideoMode::getDesktopMode().height;
                window.create(sf::VideoMode(w, h), windowName, sf::Style::Fullscreen);
            }
            else
            {
                window.create(sf::VideoMode(512, 512), windowName, sf::Style::Default);
            }
            window.setView(fixRatioCenterView());
        }
        break;
    default:
        break;
    }
}

void graphic::window::Window::createRenderingWindow()
{
    if (!window.isOpen() && (!oneWindowMode || mainWindow))
    {
        window.create(sf::VideoMode(512, 512), windowName);
        window.setFramerateLimit(fps);
        window.setView(fixRatioCenterView());
        fullscreen = false;
    }
    windowTexture.create(512, 512);
    windowTexture.setSmooth(false);
}

void graphic::window::Window::closeRenderingWindow()
{
    if (window.isOpen())
    {
        window.close();
    }
}

void graphic::window::Window::doEventFirstSubWindow(sf::Event &event)
{
    if (!subWindows.empty())
    {
        subWindows[0]->doEventFirstSubWindow(event);
    }
    else
    {
        doEvent(event);
    }
}

const sf::Texture &graphic::window::Window::getWindowTexture()
{
    if (!subWindows.empty())
    {
        return subWindows[0]->getWindowTexture();
    }
    else
    {
        return windowTexture.getTexture();
    }
}

int graphic::window::Window::getFirstSubWindowWidth()
{
    if (!subWindows.empty())
    {
        return subWindows[0]->getFirstSubWindowWidth();
    }
    else
    {
        return width;
    }
}

int graphic::window::Window::getFirstSubWindowHeight()
{
    if (!subWindows.empty())
    {
        return subWindows[0]->getFirstSubWindowHeight();
    }
    else
    {
        return height;
    }
}

void graphic::window::Window::setFirstSubWindowMousePos(sf::Vector2f mousePos)
{
    if (!subWindows.empty())
    {
        subWindows[0]->setFirstSubWindowMousePos(mousePos);
    }
    else
    {
        this->mousePos = mousePos;
    }
}

bool graphic::window::Window::getDoResize()
{
    if (!subWindows.empty())
    {
        return subWindows[0]->getDoResize() | doResize;
    }
    else
    {
        return doResize;
    }
}

void graphic::window::Window::setDoResize(bool doResize)
{
    this->doResize = doResize;
    if (!subWindows.empty())
    {
        return subWindows[0]->setDoResize(doResize);
    }
}
