#include <iostream>

#include "graphic/MainWindow.hpp"
#include "graphic/ComputerWindow.hpp"

#include "utils/console.hpp"

graphic::MainWindow::MainWindow()
{
    computer = std::make_shared<computer::Computer>();
    windowName = "S257 Dynamic Recompiler - Main Window";
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::MainWindow::MainWindow(std::string windowName)
{
    computer = std::make_shared<computer::Computer>();
    this->windowName = windowName;
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::MainWindow::MainWindow(std::string windowName, bool debug)
{
    computer = std::make_shared<computer::Computer>();
    this->windowName = windowName;
    this->debug = debug;
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::MainWindow::~MainWindow()
{
}

void graphic::MainWindow::start()
{
    printDebug("Start");

    window.create(sf::VideoMode(640, 480), windowName);
    window.setFramerateLimit(60);

    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    text.setFont(font);
    text.setCharacterSize(6);

    text.setString("Placeholder for the futur screen\nPress F1 to show another window");
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(0, 32);
    text.setPosition(pos);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());
}

void graphic::MainWindow::stop()
{
    printDebug("Stop");
}

void graphic::MainWindow::loop()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            printDebug("Closing window");
            window.close();
            run = false;
            break;
        case sf::Event::Resized:
            printDebug("Resize");
            window.setView(fixRatioCenterView());
            break;
        case sf::Event::KeyPressed:
            printDebug("Key " + std::to_string(event.key.code) + " Pressed");

            // If F1 is pressed
            if (event.key.code == sf::Keyboard::F1)
            {
                // if wa have not open the Computer window
                if (findSubWinByName(computerWindowName).empty())
                {
                    std::shared_ptr<graphic::ComputerWindow> subW = std::make_shared<graphic::ComputerWindow>(computer, computerWindowName, debug);
                    addSubWindow(subW);
                }
                else
                {
                    printDebug("Computer window already open");
                }
            }
            break;
        default:
            break;
        }
        // Clear screen
        window.clear(sf::Color(32, 32, 32));

        window.draw(rect);
        window.draw(text);

        // Update the window
        window.display();
    }
}
