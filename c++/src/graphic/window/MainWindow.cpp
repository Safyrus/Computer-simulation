#include <iostream>
#include <fstream>

#include "graphic/window/MainWindow.hpp"
#include "graphic/window/ComputerWindow.hpp"

#include "data/menu/MenuActionTest.hpp"
#include "data/menu/MenuActionOpenWindow.hpp"

#include "utils/console.hpp"
#include "utils/split.hpp"

#include "computer/ROM.hpp"

graphic::window::MainWindow::MainWindow()
{
    computer = std::make_shared<computer::Computer>(true, "prog/verifCPU/verifCPU");
    windowName = "S257 Dynamic Recompiler - Main Window";
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    showMenu = true;
    printDebug("Creation");
}

graphic::window::MainWindow::MainWindow(std::string windowName)
{
    computer = std::make_shared<computer::Computer>(true, "prog/verifCPU/verifCPU");
    this->windowName = windowName;
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    showMenu = true;
    printDebug("Creation");
}

graphic::window::MainWindow::MainWindow(std::string windowName, bool debug, std::string prog)
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

graphic::window::MainWindow::MainWindow(std::string windowName, bool debug, std::string prog, uint32_t hz)
{
    computer = std::make_shared<computer::Computer>(true, prog, hz);
    this->windowName = windowName;
    this->debug = debug;
    computerWindowName = "S257 Dynamic Recompiler - Computer Window";
    width = 256;
    height = 256;
    showMenu = true;
    printDebug("Creation");
}

graphic::window::MainWindow::~MainWindow()
{
    printDebug("Main Destruction");
}

void graphic::window::MainWindow::makeMenu()
{
    menu = std::make_shared<data::menu::Menu>();
    menu->addItem("FILE", std::make_shared<data::menu::MenuActionTest>("Not implemented yet"));
    menu->addItem("OPTION", std::make_shared<data::menu::MenuActionTest>("Not implemented yet"));
    menu->addItem("COM", std::make_shared<data::menu::MenuActionOpenWindow>(shared_from_this(), computerWindowName));

    menuView = std::make_shared<graphic::view::MenuView>(menu);
    menuView->setPos(0, 0);
    menuView->setSize(width, 6);
    menuView->setScale(2);
}

void graphic::window::MainWindow::start()
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

    if (!keyboard)
    {
        printDebug("Add keyboard");
        keyboard = std::make_shared<computer::Keyboard>();
        computer->connectIODevice(keyboard, 0);
    }

    makeMenu();

    std::shared_ptr<computer::VPU> vpu = std::static_pointer_cast<computer::VPU>(computer->getDevice("VPU", 0x1C18, 0x1C1F));
    if (vpu)
    {
        screenVpu = std::make_shared<graphic::view::ScreenVPUView>(vpu);
        screenVpu->setSize(256, 256);
    }
    else
    {
        screenVpu = nullptr;
    }

    window.setView(fixRatioCenterView());

    printDebug("Finish Starting");
}

void graphic::window::MainWindow::stop()
{
    printDebug("Wait for computer");
    while (!computer.unique())
    {
        printDebug(".");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    computer.reset();
    menu.reset();
    menuView.reset();
    screenVpu.reset();
    keyboard.reset();
    window.close();
    printDebug("Stop");
}

void graphic::window::MainWindow::loop()
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
        default:
            break;
        }
        if (keyboard)
        {
            keyboard->inputEvent(event);
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));

    //draw vpu screen
    if (screenVpu)
    {
        screenVpu->setPos(0, 0);
        screenVpu->draw(window);
    }
    else
    {
        window.draw(text);
    }

    // draw menu
    if (showMenu)
        menuView->draw(window);

    // Update the window
    window.display();
}

void graphic::window::MainWindow::openSubWindow(std::string windowName)
{
    if (windowName.compare(computerWindowName) == 0)
    {
        // if we have not open the Computer window
        if (findSubWinByName(computerWindowName).empty())
        {
            std::shared_ptr<graphic::window::ComputerWindow> subW = std::make_shared<graphic::window::ComputerWindow>(computer, computerWindowName, debug);
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

void graphic::window::MainWindow::loadConfig(std::string filePath)
{
    printDebug("Load config");

    // open the file
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
    {
        printError("Cannot open link file " + filePath);
        return;
    }
    printDebug("Config file loaded");

    // power down the computer
    printDebug("Power down computer");
    if (computer->getPower())
    {
        computer->power();
    }

    // remove all IO Devices
    printDebug("Remove IO Devices");
    for (uint8_t i = 0; i < 8; i++)
    {
        computer->removeIODevice(i);
    }

    // remove all Devices
    printDebug("Remove Devices");
    computer->removeAllDevices();

    // get Hz
    uint64_t hz = computer->getCpu()->hz;

    // Recreate the computer
    stop();
    computer = std::make_shared<computer::Computer>(false, "", hz);

    // read the csv
    // structure: device,insert,file
    printDebug("Read Config");
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> tokens = split(line, ",");
        while (tokens.size() < 3)
        {
            tokens.push_back("");
        }

        std::string device = tokens[0];
        bool insert = (tokens[1] == "1") || (tokens[1] == "true");
        std::string file = tokens[2];
        if (device == "rom4k")
        {
            if (insert)
            {
                printDebug("Create ROM4K");
                std::shared_ptr<computer::ROM> rom4k = std::make_shared<computer::ROM>(0x1000);
                rom4k->setName("ROM4K");
                computer->addDevice(rom4k, 0x0000, 0x0FFF);
                if (file != "")
                {
                    printDebug("Load ROM4K file");
                    rom4k->load(file);
                }
            }
        }
        else if (device == "rom2k")
        {
            if (insert)
            {
                printDebug("Create ROM2K");
                std::shared_ptr<computer::ROM> rom2k = std::make_shared<computer::ROM>(0x0800);
                rom2k->setName("ROM2K");
                if (file != "")
                {
                    printDebug("Load ROM2K file");
                    rom2k->load(file);
                }
                computer->addDevice(rom2k, 0x1000, 0x17FF);
            }
        }
        else if (device == "ram1k")
        {
            if (insert)
            {
                printDebug("Create RAM1K");
                std::shared_ptr<computer::RAM> ram1k = std::make_shared<computer::RAM>(0x0400);
                ram1k->setName("RAM1K");
                if (file != "")
                {
                    printDebug("Load RAM1K file");
                    ram1k->load(file);
                }
                computer->addDevice(ram1k, 0x1800, 0x1BFF);
            }
        }
        else if (device == "ram16k")
        {
            if (insert)
            {
                printDebug("Create RAM16K");
                std::shared_ptr<computer::RAM> ram16k = std::make_shared<computer::RAM>(0x4000);
                ram16k->setName("RAM16K");
                if (file != "")
                {
                    printDebug("Load RAM16K file");
                    ram16k->load(file);
                }
                computer->addDevice(ram16k, 0x4000, 0x7FFF);
            }
        }
        else if (device == "io")
        {
            if (insert)
            {
                printDebug("Create IOCTRL");
                std::shared_ptr<computer::IOController> io = std::make_shared<computer::IOController>();
                io->setName("IOCTRL");
                computer->addDevice(io, 0x1D00, 0x1D1F);
            }
        }
        else if (device == "keyboard")
        {
            if (insert)
            {
                printDebug("Add keyboard to port 0");
                keyboard = std::make_shared<computer::Keyboard>();
                computer->connectIODevice(keyboard, 0);
            }
        }
        else if (device == "vpu")
        {
            if (insert)
            {
                printDebug("Create VRAM8K");
                std::shared_ptr<computer::VRAM> vram = std::make_shared<computer::VRAM>(0x2000);
                vram->setName("VRAM8K");
                computer->addDevice(vram, 0x2000, 0x3FFF);
                if (file != "")
                {
                    printDebug("Load VRAM8K file");
                    vram->load(file);
                }

                printDebug("Create VPU");
                std::shared_ptr<computer::VPU> vpu = std::make_shared<computer::VPU>(vram);
                vpu->setName("VPU");
                computer->addDevice(vpu, 0x1C18, 0x1C1F);
            }
        }
        else if (device == "dsk")
        {
            if (insert)
            {
                printDebug("Create Floppy");
                std::shared_ptr<data::Floppy> floppy = std::make_shared<data::Floppy>();
                if(file != "")
                {
                    printDebug("Load Floppy file");
                    floppy->load(file);
                }
                printDebug("Create FDD");
                std::shared_ptr<computer::FDD> fdd = std::make_shared<computer::FDD>();
                fdd->insert(floppy);
                fdd->useLock(true);
                printDebug("Create DSKRAM512");
                std::shared_ptr<computer::VRAM> dskram = std::make_shared<computer::VRAM>(0x0200);
                dskram->setName("DSKRAM512");
                printDebug("Create FDC");
                std::shared_ptr<computer::FDC> fdc = std::make_shared<computer::FDC>(fdd, dskram);

                computer->addDevice(fdc, 0x1C08, 0x1C0F);
                computer->addDevice(dskram, 0x1E00, 0x1FFF);
            }
        }
        else if (device == "extrom32k")
        {
            if (insert)
            {
                printDebug("Create EXTROM32K");
                std::shared_ptr<computer::ROM> extrom = std::make_shared<computer::ROM>(0x8000);
                extrom->setName("EXTROM32K");
                computer->addDevice(extrom, 0x8000, 0xFFFF);
                if (file != "")
                {
                    printDebug("Load ROM4K file");
                    extrom = std::static_pointer_cast<computer::ROM>(computer->getDevice("ROM", 0x8000, 0xFFFF));
                    extrom->load(file);
                }
            }
        }
        else
        {
            printWarning("Config file: Unknow device " + device);
        }
    }

    start();

    printDebug("Config loaded");
}