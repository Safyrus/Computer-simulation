#include "graphic/ComputerWindow.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "graphic/CPUWindow.hpp"

#include "utils/console.hpp"

#include "data/menu/MenuActionTest.hpp"
#include "data/menu/MenuActionOpenWindow.hpp"
#include "data/menu/MenuActionDoubleCPUHz.hpp"
#include "data/menu/MenuActionComputerPwr.hpp"
#include "data/menu/MenuActionHalfCPUHz.hpp"
#include "data/menu/MenuActionComputerRst.hpp"

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer)
{
    this->computer = computer;
    windowName = "S257 Dynamic Recompiler - Computer Window";
    cpuWindowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName)
{
    this->computer = computer;
    this->windowName = windowName;
    cpuWindowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName, bool debug)
{
    this->debug = debug;
    this->computer = computer;
    this->windowName = windowName;
    cpuWindowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::ComputerWindow::~ComputerWindow()
{
}

void graphic::ComputerWindow::makeMenu()
{
    menu = std::make_shared<data::menu::Menu>();
    menu->addItem("PWR", std::make_shared<data::menu::MenuActionComputerPwr>(computer));
    menu->addItem("RST", std::make_shared<data::menu::MenuActionComputerRst>(computer));
    menu->addItem("HZ+", std::make_shared<data::menu::MenuActionDoubleCPUHz>(computer->getCpu()));
    menu->addItem("HZ-", std::make_shared<data::menu::MenuActionHalfCPUHz>(computer->getCpu()));
    menu->addItem("CPU", std::make_shared<data::menu::MenuActionOpenWindow>(shared_from_this(), cpuWindowName));
    menuView = std::make_shared<graphic::MenuView>(menu);
    menuView->setPos(0, 0);
    menuView->setSize(width, 6);
}

void graphic::ComputerWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::ComputerWindow::start()
{
    printDebug("Start");

    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    openTexture(com, "data/img/saphyr_I_case.png");
    openTexture(pwrOff, "data/img/saphyr1_pwr_off.png");
    openTexture(pwrOn, "data/img/saphyr1_pwr_on.png");
    openTexture(buttonOff, "data/img/saphyr1_button_off.png");
    openTexture(buttonOn, "data/img/saphyr1_button_on.png");

    text.setFont(font);
    text.setCharacterSize(6);

    text.setString("");
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(0, 32);
    text.setPosition(pos);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    makeMenu();

    window.setView(fixRatioCenterView());
}

void graphic::ComputerWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::ComputerWindow::loop()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        sf::Vector2i mousePos;
        sf::Vector2f viewMousePos;
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
        case sf::Event::MouseMoved:
            mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            viewMousePos = window.mapPixelToCoords(mousePos);
            menuView->setMousePos(viewMousePos.x, viewMousePos.y);
            break;
        case sf::Event::MouseButtonPressed:
            menuView->setMousePressed(true);
            break;
        case sf::Event::MouseButtonReleased:
            menuView->setMouseReleased(true);
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
            else if (event.key.code == sf::Keyboard::F4)
            {
                computer->reset();
            }
            break;
        default:
            break;
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));

    window.draw(rect);

    sf::Sprite caseSpr;
    // draw case
    caseSpr.setTexture(com);
    caseSpr.setPosition(sf::Vector2f(0, 6));
    window.draw(caseSpr);

    // draw light
    sf::Sprite pwrLight;
    if(computer->getPower())
        pwrLight.setTexture(pwrOn);
    else
        pwrLight.setTexture(pwrOff);
    pwrLight.setPosition(sf::Vector2f(16, 25));
    window.draw(pwrLight);

    // draw buttons
    sf::Sprite buttonPwrSprite;
    if(computer->getPower())
        buttonPwrSprite.setTexture(buttonOn);
    else
        buttonPwrSprite.setTexture(buttonOff);
    buttonPwrSprite.setPosition(sf::Vector2f(16, 13));
    window.draw(buttonPwrSprite);
    sf::Sprite buttonRstSprite;
    buttonRstSprite.setTexture(buttonOff);
    buttonRstSprite.setPosition(sf::Vector2f(48, 13));
    window.draw(buttonRstSprite);

    // display the menu
    menuView->draw(window);

    // Update the window
    window.display();
}

void graphic::ComputerWindow::openSubWindow(std::string windowName)
{
    if(windowName.compare(cpuWindowName) == 0)
    {
        // if we have not open the CPU window
        if (findSubWinByName(cpuWindowName).empty())
        {
            std::shared_ptr<graphic::CPUWindow> subW = std::make_shared<graphic::CPUWindow>(computer->getCpu(), cpuWindowName, debug);
            addSubWindow(subW);
        }
        else
        {
            std::string str = cpuWindowName + " window already open";
            printDebug(str);
        }
    }else
    {
        std::string msg = "no window with name: " + windowName;
        printDebug(msg);
    }
}
