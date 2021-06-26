#include "graphic/ComputerWindow.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "graphic/CPUWindow.hpp"
#include "graphic/RamWindow.hpp"

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
    lastDeviceNumer = 0;
    printDebug("Creation");
}

graphic::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName)
{
    this->computer = computer;
    this->windowName = windowName;
    cpuWindowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    lastDeviceNumer = 0;
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
    lastDeviceNumer = 0;
    printDebug("Creation");
}

graphic::ComputerWindow::~ComputerWindow()
{
}

void graphic::ComputerWindow::makeMenu()
{
    std::shared_ptr<data::menu::Menu> actionMenu = std::make_shared<data::menu::Menu>();
    actionMenu->addItem("PWR(F1)", std::make_shared<data::menu::MenuActionComputerPwr>(computer));
    actionMenu->addItem("HZ+(F3)", std::make_shared<data::menu::MenuActionDoubleCPUHz>(computer->getCpu()));
    actionMenu->addItem("HZ-(F2)", std::make_shared<data::menu::MenuActionHalfCPUHz>(computer->getCpu()));
    actionMenu->addItem("RST(F4)", std::make_shared<data::menu::MenuActionComputerRst>(computer));

    std::shared_ptr<data::menu::Menu> deviceMenu = std::make_shared<data::menu::Menu>();
    std::vector<std::shared_ptr<computer::Device>> devices = computer->getAllDevice();
    for (uint32_t i = 0; i < devices.size(); i++)
    {
        std::string name = devices[i]->getName();
        std::string type = devices[i]->getType();
        if (type.compare("RAM") == 0 || type.compare("VRAM") == 0)
        {
            std::stringstream indexStr;
            indexStr << std::dec << std::setfill('0') << std::setw(2) << i;
            deviceMenu->addItem(name, std::make_shared<data::menu::MenuActionOpenWindow>(shared_from_this(), "CMPWINDEV" + indexStr.str() + type));
        }
        else
        {
            deviceMenu->addItem(name, std::make_shared<data::menu::MenuActionTest>("Not implemented yet"));
        }
    }

    menu = std::make_shared<data::menu::Menu>();
    menu->addItem("ACTION", actionMenu);
    menu->addItem("CPU", std::make_shared<data::menu::MenuActionOpenWindow>(shared_from_this(), cpuWindowName));
    menu->addItem("DEVICE", deviceMenu);
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
                if (computer->getCpu()->hz / 2 > 0)
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
    if (computer->getPower())
        pwrLight.setTexture(pwrOn);
    else
        pwrLight.setTexture(pwrOff);
    pwrLight.setPosition(sf::Vector2f(16, 25));
    window.draw(pwrLight);

    // draw buttons
    sf::Sprite buttonPwrSprite;
    if (computer->getPower())
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

    if (computer->getAllDevice().size() != lastDeviceNumer)
    {
        lastDeviceNumer = computer->getAllDevice().size();
        makeMenu();
    }
}

void graphic::ComputerWindow::openSubWindow(std::string windowName)
{
    if (windowName.compare(cpuWindowName) == 0)
    {
        // if we have not open the CPU window
        if (findSubWinByName(cpuWindowName).empty())
        {
            std::shared_ptr<graphic::CPUWindow> subW = std::make_shared<graphic::CPUWindow>(computer->getCpu(), cpuWindowName, debug);
            addSubWindow(subW);
        }
        else
        {
            printDebug(cpuWindowName + " window already open");
        }
    }
    else if (windowName.substr(0, 9).compare("CMPWINDEV") == 0)
    {
        printDebug("try open device window " + windowName);
        uint16_t i = stoi(windowName.substr(9, 2), NULL, 10);
        windowName = windowName.substr(11);
        if (windowName.compare("RAM") == 0 || windowName.compare("VRAM") == 0)
        {
            std::shared_ptr<computer::RAM> ram = std::dynamic_pointer_cast<computer::RAM>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + ram->getName() + " Window";
            std::shared_ptr<graphic::RamWindow> subW = std::make_shared<graphic::RamWindow>(ram, devWinName, debug);
            printDebug("open ram");
            addSubWindow(subW);
        }
        else
        {
            printDebug("no device window with name: " + windowName);
        }
    }
    else
    {
        printDebug("no window with name: " + windowName);
    }
}
