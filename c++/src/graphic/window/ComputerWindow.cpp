#include "graphic/window/ComputerWindow.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "graphic/window/CPUWindow.hpp"
#include "graphic/window/RamWindow.hpp"
#include "graphic/window/RomWindow.hpp"
#include "graphic/window/VramWindow.hpp"
#include "graphic/window/IOControllerWindow.hpp"
#include "graphic/window/HwStatesWindow.hpp"
#include "graphic/window/VPUWindow.hpp"
#include "graphic/window/FDCWindow.hpp"

#include "utils/console.hpp"

#include "data/menu/MenuActionTest.hpp"
#include "data/menu/MenuActionOpenWindow.hpp"
#include "data/menu/MenuActionDoubleCPUHz.hpp"
#include "data/menu/MenuActionComputerPwr.hpp"
#include "data/menu/MenuActionHalfCPUHz.hpp"
#include "data/menu/MenuActionComputerRst.hpp"
#include "data/menu/MenuActionUseFDDLock.hpp"
#include "data/menu/MenuActionUseFDDFloppy.hpp"

graphic::window::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer)
{
    this->computer = computer;
    windowName = "S257 Dynamic Recompiler - Computer Window";
    cpuWindowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    lastDeviceNumer = 0;
    printDebug("Creation");
}

graphic::window::ComputerWindow::ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName, bool debug, bool oneWindow)
{
    this->debug = debug;
    this->computer = computer;
    this->windowName = windowName;
    this->oneWindowMode = oneWindow;
    cpuWindowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    lastDeviceNumer = 0;
    printDebug("Creation");
}

graphic::window::ComputerWindow::~ComputerWindow()
{
}

void graphic::window::ComputerWindow::makeMenu()
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
        if (type.compare("RAM") == 0 || type.compare("VRAM") == 0 || type.compare("ROM") == 0 || type.compare("IOCTRL") == 0 || type.compare("HWSTATS") == 0 || type.compare("VPU") == 0 || type.compare("FDC") == 0)
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

    std::shared_ptr<data::menu::Menu> floppyMenu = std::make_shared<data::menu::Menu>();
    std::shared_ptr<computer::FDC> fdc = std::static_pointer_cast<computer::FDC>(computer->getDevice("FDC", 0x0000, 0xFFFF));
    std::shared_ptr<computer::FDD> fdd = nullptr;
    if (fdc)
    {
        fdd = fdc->getFDD();
    }
    floppyMenu->addItem("USE LOCK", std::make_shared<data::menu::MenuActionUseFDDLock>(fdd));
    floppyMenu->addItem("LOAD/EJECT FLOPPY", std::make_shared<data::menu::MenuActionUseFDDFloppy>(fdd));

    menu = std::make_shared<data::menu::Menu>();
    menu->addItem("ACTION", actionMenu);
    menu->addItem("CPU", std::make_shared<data::menu::MenuActionOpenWindow>(shared_from_this(), cpuWindowName));
    menu->addItem("DEVICE", deviceMenu);
    menu->addItem("FLOPPY", floppyMenu);
    menuView = std::make_shared<graphic::view::MenuView>(menu);
    menuView->setPos(0, 0);
    menuView->setSize(width, 6);
}

void graphic::window::ComputerWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::window::ComputerWindow::start()
{
    printDebug("Start");

    createRenderingWindow();

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
    openTexture(dskOff, "data/img/saphyr1_dsk_off.png");
    openTexture(dskOn, "data/img/saphyr1_dsk_on.png");
    openTexture(floppyOff, "data/img/saphyr1_floppy_off.png");
    openTexture(floppyOn, "data/img/saphyr1_floppy_on.png");
    openTexture(floppyDsk, "data/img/saphyr1_floppy_dsk.png");

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

    cursorOnPWR = false;
    cursorOnRST = false;
    cursorOnLock = false;
    mousePressed = false;
}

void graphic::window::ComputerWindow::stop()
{
    computer.reset();
    menu.reset();
    menuView.reset();
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::ComputerWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));

    windowTexture.draw(rect);

    sf::Sprite caseSprite;
    // draw case
    caseSprite.setTexture(com);
    caseSprite.setPosition(sf::Vector2f(0, 6));
    windowTexture.draw(caseSprite);

    // draw pwr light
    sf::Sprite pwrLightSprite;
    if (computer->getPower())
        pwrLightSprite.setTexture(pwrOn);
    else
        pwrLightSprite.setTexture(pwrOff);
    pwrLightSprite.setPosition(sf::Vector2f(12, 25));
    windowTexture.draw(pwrLightSprite);

    // draw pwr buttons
    sf::Sprite buttonPwrSprite;
    if (computer->getPower() || (cursorOnPWR && mousePressed))
        buttonPwrSprite.setTexture(buttonOn);
    else
        buttonPwrSprite.setTexture(buttonOff);
    buttonPwrSprite.setPosition(sf::Vector2f(12, 14));
    windowTexture.draw(buttonPwrSprite);

    // draw rst buttons
    sf::Sprite buttonRstSprite;
    if (cursorOnRST && mousePressed)
    {
        buttonRstSprite.setTexture(buttonOn);
    }
    else
    {
        buttonRstSprite.setTexture(buttonOff);
    }
    buttonRstSprite.setPosition(sf::Vector2f(12, 32));
    windowTexture.draw(buttonRstSprite);

    // draw floppy disk
    if (computer->isFloppyIn())
    {
        sf::Sprite floppyDiskSprite;
        floppyDiskSprite.setTexture(floppyDsk);
        floppyDiskSprite.setPosition(sf::Vector2f(53, 36));
        windowTexture.draw(floppyDiskSprite);
    }

    // draw floppy drive
    sf::Sprite floppyDriveSprite;
    if (computer->isDriveLock())
        floppyDriveSprite.setTexture(floppyOn);
    else
        floppyDriveSprite.setTexture(floppyOff);
    floppyDriveSprite.setPosition(sf::Vector2f(52, 32));
    windowTexture.draw(floppyDriveSprite);

    // draw floppy drive light
    sf::Sprite dskSprite;
    if (computer->isDriveActive())
        dskSprite.setTexture(dskOn);
    else
        dskSprite.setTexture(dskOff);
    dskSprite.setPosition(sf::Vector2f(56, 32));
    windowTexture.draw(dskSprite);

    // display the menu
    menuView->draw(windowTexture);

    // Update the window
    windowTexture.display();

    if (computer->getAllDevice().size() != lastDeviceNumer)
    {
        lastDeviceNumer = computer->getAllDevice().size();
        makeMenu();
    }
}

void graphic::window::ComputerWindow::doEvent(sf::Event &event)
{
    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::MouseMoved:
        menuView->setMousePos(mousePos.x, mousePos.y);

        // PWR button collision
        cursorOnPWR = (mousePos.x >= 12 && mousePos.x <= 20 && mousePos.y >= 8 + 6 && mousePos.y <= 15 + 6);

        // RST button collision
        cursorOnRST = (mousePos.x >= 12 && mousePos.x <= 20 && mousePos.y >= 26 + 6 && mousePos.y <= 33 + 6);

        // Lock button collision
        if (computer->isDriveLock())
        {
            cursorOnLock = (mousePos.x >= 62 && mousePos.x <= 74 && mousePos.y >= 26 + 6 && mousePos.y <= 33 + 6);
            cursorOnFloppy = false;
        }
        else
        {
            cursorOnLock = (mousePos.x >= 62 && mousePos.x <= 74 && mousePos.y >= 26 + 6 && mousePos.y <= 29 + 6);
            cursorOnFloppy = (mousePos.x >= 52 && mousePos.x <= 84 && mousePos.y >= 29 + 6 && mousePos.y <= 31 + 6);
        }
        break;
    case sf::Event::MouseButtonPressed:
        menuView->setMousePressed(true);
        mousePressed = true;
        break;
    case sf::Event::MouseButtonReleased:
        menuView->setMouseReleased(true);
        mousePressed = false;
        if (cursorOnPWR)
        {
            computer->power();
        }
        if (cursorOnRST)
        {
            computer->reset();
        }

        if (cursorOnLock)
        {
            std::shared_ptr<computer::FDC> fdc = std::static_pointer_cast<computer::FDC>(computer->getDevice("FDC", 0x0000, 0xFFFF));
            if (fdc)
            {
                std::shared_ptr<computer::FDD> fdd = fdc->getFDD();
                if (fdd)
                {
                    fdd->useLock(!fdd->isLock());
                }
            }
        }
        if (cursorOnFloppy)
        {
            std::shared_ptr<computer::FDC> fdc = std::static_pointer_cast<computer::FDC>(computer->getDevice("FDC", 0x0000, 0xFFFF));
            if (fdc)
            {
                std::shared_ptr<computer::FDD> fdd = fdc->getFDD();
                if (fdd)
                {
                    if (fdd->isFloppyIn())
                    {
                        fdd->eject();
                    }
                    else
                    {
                        std::shared_ptr<data::Floppy> floppy = std::make_shared<data::Floppy>();
                        floppy->load("floppy.img");
                        fdd->insert(floppy);
                    }
                }
            }
        }
        break;
    case sf::Event::KeyPressed:
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
        else if (event.key.code == sf::Keyboard::Escape)
        {
            run = false;
        }
        break;
    default:
        break;
    }
}

void graphic::window::ComputerWindow::openSubWindow(std::string windowName)
{
    if (windowName.compare(cpuWindowName) == 0)
    {
        // if we have not open the CPU window
        if (findSubWinByName(cpuWindowName).empty())
        {
            std::shared_ptr<graphic::window::CPUWindow> subW = std::make_shared<graphic::window::CPUWindow>(computer->getCpu(), cpuWindowName, debug);
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

        if (windowName.compare("RAM") == 0)
        {
            std::shared_ptr<computer::RAM> ram = std::dynamic_pointer_cast<computer::RAM>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + ram->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::RamWindow> subW = std::make_shared<graphic::window::RamWindow>(ram, devWinName, debug);
            printDebug("open ram");
            addSubWindow(subW);
        }
        else if (windowName.compare("VRAM") == 0)
        {
            std::shared_ptr<computer::VRAM> vram = std::dynamic_pointer_cast<computer::VRAM>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + vram->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::VramWindow> subW = std::make_shared<graphic::window::VramWindow>(vram, devWinName, debug);
            printDebug("open vram");
            addSubWindow(subW);
        }
        else if (windowName.compare("ROM") == 0)
        {
            std::shared_ptr<computer::ROM> rom = std::dynamic_pointer_cast<computer::ROM>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + rom->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::RomWindow> subW = std::make_shared<graphic::window::RomWindow>(rom, devWinName, debug);
            printDebug("open rom");
            addSubWindow(subW);
        }
        else if (windowName.compare("IOCTRL") == 0)
        {
            std::shared_ptr<computer::IOController> io = std::dynamic_pointer_cast<computer::IOController>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + io->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::IOControllerWindow> subW = std::make_shared<graphic::window::IOControllerWindow>(io, devWinName, debug);
            printDebug("open ioCtrl");
            addSubWindow(subW);
        }
        else if (windowName.compare("HWSTATS") == 0)
        {
            std::shared_ptr<computer::HardwareStates> hwstats = std::dynamic_pointer_cast<computer::HardwareStates>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + hwstats->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::HwStatesWindow> subW = std::make_shared<graphic::window::HwStatesWindow>(hwstats, devWinName, debug);
            printDebug("open hwStats");
            addSubWindow(subW);
        }
        else if (windowName.compare("VPU") == 0)
        {
            std::shared_ptr<computer::VPU> vpu = std::dynamic_pointer_cast<computer::VPU>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + vpu->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::VPUWindow> subW = std::make_shared<graphic::window::VPUWindow>(vpu, devWinName, debug);
            printDebug("open hwStats");
            addSubWindow(subW);
        }
        else if (windowName.compare("FDC") == 0)
        {
            std::shared_ptr<computer::FDC> fdc = std::dynamic_pointer_cast<computer::FDC>(computer->getAllDevice()[i]);
            std::string devWinName = "S257 Dynamic Recompiler - " + fdc->getName() + " Window";
            if (!findSubWinByName(devWinName).empty())
            {
                printDebug(devWinName + " window already open");
                return;
            }
            std::shared_ptr<graphic::window::FDCWindow> subW = std::make_shared<graphic::window::FDCWindow>(fdc, devWinName, debug);
            printDebug("open hwStats");
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
