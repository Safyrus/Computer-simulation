#include "graphic/ComputerWindow.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "utils/console.hpp"

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer)
{
    this->computer = computer;
    windowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName)
{
    this->computer = computer;
    this->windowName = windowName;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName, bool debug)
{
    this->debug = debug;
    this->computer = computer;
    this->windowName = windowName;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::ComputerWindow::~ComputerWindow()
{
}

void graphic::ComputerWindow::start()
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

    text.setString("");
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(0, 32);
    text.setPosition(pos);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());
}

void graphic::ComputerWindow::stop()
{
    printDebug("Stop");
}

void graphic::ComputerWindow::loop()
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
            if (event.key.code == sf::Keyboard::F1)
            {
                computer->power();
            }
            else if (event.key.code == sf::Keyboard::F2)
            {
                if(computer->getCpu()->hz / 2 > 0)
                {
                    computer->getCpu()->hz /= 2;
                }
            }
            else if (event.key.code == sf::Keyboard::F3)
            {
                computer->getCpu()->hz *= 2;
            }
            break;
        default:
            break;
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));

    window.draw(rect);

    // set the computer infos string
    std::stringstream txt;
    txt << "Computer:\n";
    txt << "  PWR  : " << ((computer->getPower()) ? "ON" : "OFF") << "\n";
    txt << "  HZ   : " << computer->getCpu()->hz << "\n";
    txt << "  CYCLE: " << computer->getCpu()->cycle << "\n";
    txt << "  ADR  : " << std::hex << std::setfill('0') << std::setw(4) << computer->getCpu()->pc << "\n";
    txt << "  REG  : O  A  B  C  D  E  F  R  J1 J2 G0 G1 G2 G3 G4 G5\n         ";
    for (unsigned int i = 0; i < 16; i++)
    {
        txt << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)computer->getCpu()->reg[i] << " ";
    }

    // display the computer infos
    text.setString(txt.str());
    window.draw(text);

    // Update the window
    window.display();
}
