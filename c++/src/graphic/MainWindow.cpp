#include <iostream>

#include "graphic/MainWindow.hpp"
#include "graphic/ComputerWindow.hpp"
//#include "graphic/ScreenSimpleView.hpp"

#include "data/menu/MenuActionTest.hpp"
#include "data/menu/MenuActionOpenWindow.hpp"

#include "utils/console.hpp"

graphic::MainWindow::MainWindow()
{
    computer = std::make_shared<computer::Computer>(true, "prog/verifCPU/verifCPU");
    windowName = "S257 Dynamic Recompiler - Main Window";
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    showMenu = true;
    printDebug("Creation");
}

graphic::MainWindow::MainWindow(std::string windowName)
{
    computer = std::make_shared<computer::Computer>(true, "prog/verifCPU/verifCPU");
    this->windowName = windowName;
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    showMenu = true;
    printDebug("Creation");
}

graphic::MainWindow::MainWindow(std::string windowName, bool debug, std::string prog)
{
    computer = std::make_shared<computer::Computer>(true, prog);
    this->windowName = windowName;
    this->debug = debug;
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    showMenu = true;
    printDebug("Creation");
}

graphic::MainWindow::~MainWindow()
{
}

void graphic::MainWindow::makeMenu()
{
    std::shared_ptr<data::menu::Menu> subSubMenu = std::make_shared<data::menu::Menu>();
    subSubMenu->addItem("789", std::make_shared<data::menu::MenuActionTest>("789"));
    subSubMenu->addItem("456", std::make_shared<data::menu::MenuActionTest>("456"));

    std::shared_ptr<data::menu::Menu> subMenu = std::make_shared<data::menu::Menu>();
    subMenu->addItem("123", std::make_shared<data::menu::MenuActionTest>("123"));
    subMenu->addItem("MENU", subSubMenu);

    menu = std::make_shared<data::menu::Menu>();
    menu->addItem("FILE", std::make_shared<data::menu::MenuActionTest>("Not implemented yet"));
    menu->addItem("OPTION", std::make_shared<data::menu::MenuActionTest>("Not implemented yet"));
    menu->addItem("COM", std::make_shared<data::menu::MenuActionOpenWindow>(shared_from_this(), computerWindowName));
    menu->addItem("SUBMENU", subMenu);

    menuView = std::make_shared<graphic::MenuView>(menu);
    menuView->setPos(0, 0);
    menuView->setSize(width, 6);
    menuView->setScale(2);
}

void graphic::MainWindow::start()
{
    printDebug("Start");

    window.create(sf::VideoMode(512, 512), windowName);
    window.setFramerateLimit(60);

    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    text.setFont(font);
    text.setCharacterSize(6);

    text.setString("NO SCREEN FOUND");
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(4, 12);
    text.setPosition(pos);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    makeMenu();

    std::shared_ptr<computer::VPU> vpu = std::static_pointer_cast<computer::VPU>(computer->getDevice("VPU", 0x1C18, 0x1C1F));
    screenVpu = std::make_shared<graphic::ScreenVPUView>(vpu);
    screenVpu->setSize(256, 256);

    window.setView(fixRatioCenterView());
}

void graphic::MainWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::MainWindow::loop()
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

            // If F1 is pressed
            if (event.key.code == sf::Keyboard::F1)
            {
                openSubWindow(computerWindowName);
            }
            // If F2 is pressed
            else if (event.key.code == sf::Keyboard::F2)
            {
                showMenu = !showMenu;
            }
            break;
        default:
            break;
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));

    /*
    std::shared_ptr<computer::RAM> ram = std::static_pointer_cast<computer::RAM>(computer->getDevice("RAM", 0x2000, 0x3FFF));
    if (ram != nullptr)
    {
        //draw screen
        ScreenSimpleView screen(ram);
        screen.setBackgroundColor(sf::Color(0, 32, 0));
        screen.setColor(sf::Color(0, 170, 0));
        screen.setPos(0, 0);
        screen.draw(window);
    }
    else
    {
        // draw text
        window.draw(rect);
        window.draw(text);
    }
    */
    //draw vpu screen
    screenVpu->setPos(0, 0);
    screenVpu->draw(window);

    // draw menu
    if (showMenu)
        menuView->draw(window);

    // Update the window
    window.display();
}

void graphic::MainWindow::openSubWindow(std::string windowName)
{
    if (windowName.compare(computerWindowName) == 0)
    {
        // if we have not open the Computer window
        if (findSubWinByName(computerWindowName).empty())
        {
            std::shared_ptr<graphic::ComputerWindow> subW = std::make_shared<graphic::ComputerWindow>(computer, computerWindowName, debug);
            addSubWindow(subW);
        }
        else
        {
            std::string str = computerWindowName + " window already open";
            printDebug(str);
        }
    }
    else
    {
        std::string msg = "no window with name: " + windowName;
        printDebug(msg);
    }
}
