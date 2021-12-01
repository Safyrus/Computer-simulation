#include "graphic/window/DebugerWindow.hpp"

#include "data/menu/Menu.hpp"
#include "data/menu/MenuActionCPUPause.hpp"
#include "data/menu/MenuActionCPUStep.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

std::map<uint8_t, std::string> graphic::window::DebugerWindow::REGNAME = {{0x0, "0 "}, {0x1, "A "}, {0x2, "B "}, {0x3, "C "}, {0x4, "D "}, {0x5, "E "}, {0x6, "F "}, {0x7, "R "}, {0x8, "J1"}, {0x9, "J2"}, {0xA, "G0"}, {0xB, "G1"}, {0xC, "G2"}, {0xD, "G3"}, {0xE, "G4"}, {0xF, "G5"}};
//const std::map<uint8_t, std::string> graphic::window::DebugerWindow::ARITHMETICCMDNAME = {{0x0, "ADD"}, {0x1, "ADC"}, {0x2, "SUB"}, {0x3, "SBB"}, {0x4, "MUL"}, {0x5, "DIV"}, {0x6, "MOD"}, {0x7, "AND"}, {0x8, "OR "}, {0x9, "XOR"}};
std::map<uint8_t, std::string> graphic::window::DebugerWindow::CMDS = {
    {0x00, "NOP"}, {0x01, "RST"}, {0x02, "OFF"}, {0x03, "MOV"}, {0x04, "MOV"}, {0x05, "CMP"}, {0x06, "CMP"}, {0x07, "CMP"}, {0x08, "CMP"}, {0x10, "ADD"}, {0x11, "ADC"}, {0x12, "SUB"}, {0x13, "SBB"}, {0x14, "MUL"}, {0x15, "DIV"}, {0x16, "MOD"}, {0x17, "AND"}, {0x18, "OR "}, {0x19, "XOR"}, {0x20, "ADD"}, {0x21, "ADC"}, {0x22, "SUB"}, {0x23, "SBB"}, {0x24, "MUL"}, {0x25, "DIV"}, {0x26, "MOD"}, {0x27, "AND"}, {0x28, "OR "}, {0x29, "XOR"}, {0x30, "ADD"}, {0x31, "ADC"}, {0x32, "SUB"}, {0x33, "SBB"}, {0x34, "MUL"}, {0x35, "DIV"}, {0x36, "MOD"}, {0x37, "AND"}, {0x38, "OR "}, {0x39, "XOR"}, {0x40, "ADD"}, {0x41, "ADC"}, {0x42, "SUB"}, {0x43, "SBB"}, {0x44, "MUL"}, {0x45, "DIV"}, {0x46, "MOD"}, {0x47, "AND"}, {0x48, "OR "}, {0x49, "XOR"}, {0x50, "JMP"}, {0x51, "JMP"}, {0x52, "JMP"}, {0x53, "JMP"}, {0x54, "JMP"}, {0x55, "JMP"}, {0x56, "JMP"}, {0x57, "JMP"}, {0x60, "GET"}, {0x61, "GET"}, {0x62, "GET"}, {0x63, "GET"}, {0x64, "SET"}, {0x65, "SET"}, {0x66, "SET"}, {0x67, "SET"}, {0x68, "SET"}, {0x69, "SET"}, {0x6A, "SET"}, {0x6B, "SET"}};
std::map<uint8_t, bool> graphic::window::DebugerWindow::DSTREG = {
    {0x00, false}, {0x01, false}, {0x02, false}, {0x03, true}, {0x04, true}, {0x05, false}, {0x06, false}, {0x07, false}, {0x08, false}, {0x10, true}, {0x11, true}, {0x12, true}, {0x13, true}, {0x14, true}, {0x15, true}, {0x16, true}, {0x17, true}, {0x18, true}, {0x19, true}, {0x20, true}, {0x21, true}, {0x22, true}, {0x23, true}, {0x24, true}, {0x25, true}, {0x26, true}, {0x27, true}, {0x28, true}, {0x29, true}, {0x30, true}, {0x31, true}, {0x32, true}, {0x33, true}, {0x34, true}, {0x35, true}, {0x36, true}, {0x37, true}, {0x38, true}, {0x39, true}, {0x40, true}, {0x41, true}, {0x42, true}, {0x43, true}, {0x44, true}, {0x45, true}, {0x46, true}, {0x47, true}, {0x48, true}, {0x49, true}, {0x50, true}, {0x51, true}, {0x52, true}, {0x53, true}, {0x54, false}, {0x55, false}, {0x56, false}, {0x57, false}, {0x60, true}, {0x61, true}, {0x62, true}, {0x63, true}, {0x64, true}, {0x65, true}, {0x66, true}, {0x67, true}, {0x68, false}, {0x69, false}, {0x6A, false}, {0x6B, false}};
std::map<uint8_t, bool> graphic::window::DebugerWindow::SRCREG = {
    {0x00, false}, {0x01, false}, {0x02, false}, {0x03, true}, {0x04, false}, {0x05, true}, {0x06, false}, {0x07, true}, {0x08, false}, {0x10, true}, {0x11, true}, {0x12, true}, {0x13, true}, {0x14, true}, {0x15, true}, {0x16, true}, {0x17, true}, {0x18, true}, {0x19, true}, {0x20, false}, {0x21, false}, {0x22, false}, {0x23, false}, {0x24, false}, {0x25, false}, {0x26, false}, {0x27, false}, {0x28, false}, {0x29, false}, {0x30, true}, {0x31, true}, {0x32, true}, {0x33, true}, {0x34, true}, {0x35, true}, {0x36, true}, {0x37, true}, {0x38, true}, {0x39, true}, {0x40, false}, {0x41, false}, {0x42, false}, {0x43, false}, {0x44, false}, {0x45, false}, {0x46, false}, {0x47, false}, {0x48, false}, {0x49, false}, {0x50, true}, {0x51, false}, {0x52, true}, {0x53, false}, {0x54, true}, {0x55, false}, {0x56, true}, {0x57, false}, {0x60, true}, {0x61, false}, {0x62, true}, {0x63, false}, {0x64, true}, {0x65, false}, {0x66, true}, {0x67, false}, {0x68, true}, {0x69, false}, {0x6A, true}, {0x6B, false}};
std::map<uint8_t, bool> graphic::window::DebugerWindow::VALREG = {
    {0x00, false}, {0x01, false}, {0x02, false}, {0x03, false}, {0x04, false}, {0x05, true}, {0x06, true}, {0x07, false}, {0x08, false}, {0x10, true}, {0x11, true}, {0x12, true}, {0x13, true}, {0x14, true}, {0x15, true}, {0x16, true}, {0x17, true}, {0x18, true}, {0x19, true}, {0x20, true}, {0x21, true}, {0x22, true}, {0x23, true}, {0x24, true}, {0x25, true}, {0x26, true}, {0x27, true}, {0x28, true}, {0x29, true}, {0x30, false}, {0x31, false}, {0x32, false}, {0x33, false}, {0x34, false}, {0x35, false}, {0x36, false}, {0x37, false}, {0x38, false}, {0x39, false}, {0x40, false}, {0x41, false}, {0x42, false}, {0x43, false}, {0x44, false}, {0x45, false}, {0x46, false}, {0x47, false}, {0x48, false}, {0x49, false}, {0x50, true}, {0x51, true}, {0x52, false}, {0x53, false}, {0x54, true}, {0x55, true}, {0x56, false}, {0x57, false}, {0x60, true}, {0x61, true}, {0x62, false}, {0x63, false}, {0x64, true}, {0x65, true}, {0x66, false}, {0x67, false}, {0x68, true}, {0x69, true}, {0x6A, false}, {0x6B, false}};

graphic::window::DebugerWindow::DebugerWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName, bool debug)
{
    this->cpu = cpu;
    this->debug = debug;
    this->windowName = windowName;
    width = 256;
    height = 256;
    printDebug("Creation");
}

graphic::window::DebugerWindow::~DebugerWindow()
{
}

void graphic::window::DebugerWindow::writeHex(std::stringstream &str, uint16_t adr)
{
    str << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (uint16_t)cpu->get(adr) << "  ";
}

void graphic::window::DebugerWindow::writeNewLine(std::stringstream &str, uint16_t adr)
{
    // write new line
    str << "\n";

    //write adr
    str << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << adr;

    // write breakpoint marker
    std::vector<uint16_t> breakpoints = cpu->getBreakpoints();
    bool break1 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 0)) != breakpoints.end());
    bool break2 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 1)) != breakpoints.end());
    bool break3 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 2)) != breakpoints.end());
    bool break4 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 3)) != breakpoints.end());
    bool findBreak = (break1 || break2 || break3 || break4);
    if (findBreak)
    {
        str << " " << (char)0xB7 << " ";
    }
    else
    {
        str << "   ";
    }
}

void graphic::window::DebugerWindow::writeInstruction(std::stringstream &str, uint16_t adr)
{
    // get current instruction
    uint8_t cmd = cpu->get(adr + 0);
    uint8_t dst = cpu->get(adr + 1);
    uint8_t src = cpu->get(adr + 2);
    uint8_t val = cpu->get(adr + 3);

    // write cmd
    str << CMDS[cmd] << " ";

    // write dst
    if (DSTREG[cmd])
        str << "$" << REGNAME[dst] << " ";
    else
        writeHex(str, adr + 1);

    // write src
    if (SRCREG[cmd])
        str << "$" << REGNAME[src] << " ";
    else
        writeHex(str, adr + 2);

    // write val
    if (VALREG[cmd])
        str << "$" << REGNAME[val] << " ";
    else
        writeHex(str, adr + 3);
}

bool graphic::window::DebugerWindow::isInstruction(uint16_t adr)
{
    // get the possible instruction
    uint8_t cmd = cpu->get(adr);
    uint8_t dst = cpu->get(adr + 1);
    uint8_t src = cpu->get(adr + 2);
    uint8_t val = cpu->get(adr + 3);

    std::string str = CMDS[cmd];
    if (str.compare("") != 0)
    {
        // check if dst need to be 0
        if (cmd <= 0x08 && cmd != 0x03 && cmd != 0x04 && dst != 0)
        {
            return false;
        }
        // check if src need to be 0
        if (cmd <= 0x02 && src != 0)
        {
            return false;
        }
        // check if val need to be 0
        if (cmd <= 0x04 && val != 0)
        {
            return false;
        }

        // check if dst is a reg
        if (DSTREG[cmd] && dst >= 0x10)
        {
            return false;
        }
        // check if src is a reg
        if (SRCREG[cmd] && dst >= 0x10)
        {
            return false;
        }
        // check if val is a reg
        if (VALREG[cmd] && dst >= 0x10)
        {
            return false;
        }
        return true;
    }
    return false;
}

void graphic::window::DebugerWindow::drawCPUpc(int line, uint16_t adr)
{
    if (cpu->isPause() && cpu->pc == adr)
    {
        sf::RectangleShape PCrect;
        PCrect.setFillColor(sf::Color(0x29, 0x36, 0x6f));
        PCrect.setSize(sf::Vector2f(16 * 4, 1 * 6));
        PCrect.setPosition(4 + (7 * 4), 12 + (line * 6));
        windowTexture.draw(PCrect);
    }
}

void graphic::window::DebugerWindow::drawBreaks(int line, uint16_t adr)
{
    rect.setOutlineThickness(0);
    rect.setSize(sf::Vector2f(4 * 4, 1 * 6));
    rect.setFillColor(sf::Color(0x25, 0x71, 0x79));

    std::vector<uint16_t> breakpoints = cpu->getBreakpoints();

    bool break1 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 0)) != breakpoints.end());
    bool break2 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 1)) != breakpoints.end());
    bool break3 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 2)) != breakpoints.end());
    bool break4 = (std::find(breakpoints.begin(), breakpoints.end(), (uint16_t)(adr + 3)) != breakpoints.end());

    bool find = (break1 || break2 || break3 || break4);
    if (find)
    {
        if (break1)
        {
            rect.setPosition(4 + (7 * 4) + (0 * 16), 12 + (line * 6));
            windowTexture.draw(rect);
        }
        if (break2)
        {
            rect.setPosition(4 + (7 * 4) + (1 * 16), 12 + (line * 6));
            windowTexture.draw(rect);
        }
        if (break3)
        {
            rect.setPosition(4 + (7 * 4) + (2 * 16), 12 + (line * 6));
            windowTexture.draw(rect);
        }
        if (break4)
        {
            rect.setPosition(4 + (7 * 4) + (3 * 16), 12 + (line * 6));
            windowTexture.draw(rect);
        }
    }
}

void graphic::window::DebugerWindow::drawInstructions()
{
    int nbLine = 39;

    // draw menu background
    rect.setFillColor(sf::Color(0x1a, 0x1c, 0x2c));
    rect.setSize(sf::Vector2f(256, 7));
    rect.setPosition(0, 0);
    rect.setOutlineThickness(0);
    windowTexture.draw(rect);

    // draw left background
    rect.setFillColor(sf::Color(0x25, 0x71, 0x79));
    rect.setSize(sf::Vector2f(7 * 4, nbLine * 6));
    rect.setPosition(4, 12);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color(0x1a, 0x1c, 0x2c));
    windowTexture.draw(rect);

    // draw right background
    rect.setFillColor(sf::Color(0x33, 0x3c, 0x57));
    rect.setSize(sf::Vector2f(16 * 4, nbLine * 6));
    rect.setPosition(4 + (7 * 4), 12);
    windowTexture.draw(rect);

    if (cpu->isPause() && (!cpuHasPaused || (cpu->pc >= scrollAdr && (cpu->pc + 4 * nbLine) < scrollAdr)))
    {
        scrollAdr = cpu->pc - ((nbLine*4)/2);
        cpuHasPaused = true;
    }
    lastScrollAdr = scrollAdr;

    std::stringstream str;
    int i = 0;
    int l = 0;
    int lastHex = 0;
    writeNewLine(str, scrollAdr + i);
    while (l < nbLine)
    {
        if (isInstruction(scrollAdr + i) && !hexDisplay)
        {
            if (lastHex)
            {
                lastHex = 0;
                l++;
                if (l < nbLine)
                {
                    writeNewLine(str, scrollAdr + i);
                    drawCPUpc(l, scrollAdr + i);
                    drawBreaks(l, scrollAdr + i);
                }
            }
            if (l < nbLine)
            {
                writeInstruction(str, scrollAdr + i);
                drawCPUpc(l, scrollAdr + i);
                drawBreaks(l, scrollAdr + i);
            }
            l++;
            i += 4;
            if (l < nbLine)
            {
                writeNewLine(str, scrollAdr + i);
                drawCPUpc(l, scrollAdr + i);
                drawBreaks(l, scrollAdr + i);
            }
        }
        else
        {
            if (lastHex == 4)
            {
                lastHex = 0;
                l++;
                if (l < nbLine)
                {
                    writeNewLine(str, scrollAdr + i);
                    drawCPUpc(l, scrollAdr + i);
                    drawBreaks(l, scrollAdr + i);
                }
            }
            if (l < nbLine)
            {
                writeHex(str, scrollAdr + i);
                drawCPUpc(l, scrollAdr + i);
                drawBreaks(l, scrollAdr + i);
            }
            i++;
            lastHex++;
        }
    }

    txt.setFillColor(sf::Color(0xf4, 0xf4, 0xf4));
    txt.setString(str.str());
    txt.setPosition(4, 6);
    windowTexture.draw(txt);
}

void graphic::window::DebugerWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    txt.setFont(font);
    txt.setCharacterSize(6);

    std::shared_ptr<data::menu::Menu> menu = std::make_shared<data::menu::Menu>();
    menu->addItem("PAUSE(F1)", std::make_shared<data::menu::MenuActionCPUPause>(cpu));
    menu->addItem("STEP(F2)", std::make_shared<data::menu::MenuActionCPUStep>(cpu));
    menuView = std::make_shared<graphic::view::MenuView>(menu);
    menuView->setPos(0, 0);
    menuView->setSize(width, 6);

    breakpointInput = std::make_shared<graphic::view::TextInputView>();
    breakpointInput->setPos(149, 20);
    breakpointInput->setSize(22, 7);
    breakpointInput->setLimit(4);
    breakpointInput->setActive(false);

    scrollInput = std::make_shared<graphic::view::TextInputView>();
    scrollInput->setPos(149, 44);
    scrollInput->setSize(22, 7);
    scrollInput->setLimit(4);
    scrollInput->setActive(false);

    scrollAdr = 0;
    cpuHasPaused = false;
    hexDisplay = false;
}

void graphic::window::DebugerWindow::stop()
{
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::DebugerWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));
    rect.setPosition(0, 0);
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor(sf::Color(0x56, 0x6c, 0x86));
    windowTexture.draw(rect);

    drawInstructions();

    // display text inputs
    breakpointInput->draw(windowTexture);
    scrollInput->draw(windowTexture);

    // display the text
    std::stringstream str;
    str << "PLACE/REMOVE BREAKPOINT:\n\n\n\n";
    str << "SCROLL TO:\n\n\n\n";
    str << "IS PAUSED: " << ((cpu->isPause()) ? "YES" : "NO") << "\n\n";
    str << "HEX DISPLAY(F3): " << ((hexDisplay) ? "YES" : "NO") << "\n\n";
    txt.setFillColor(sf::Color(0xf4, 0xf4, 0xf4));
    txt.setPosition(132, 12);
    txt.setString(str.str());
    windowTexture.draw(txt);

    // display text inputs name
    txt.setString("ADR:");
    txt.setPosition(132, 21);
    windowTexture.draw(txt);
    txt.setPosition(132, 45);
    windowTexture.draw(txt);

    // display the menu
    menuView->draw(windowTexture);

    // Update the window
    windowTexture.display();
}

void graphic::window::DebugerWindow::doEvent(sf::Event &event)
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::MouseMoved:
        menuView->setMousePos(mousePos.x, mousePos.y);
        break;
    case sf::Event::MouseButtonPressed:
        menuView->setMousePressed(true);
        break;
    case sf::Event::MouseButtonReleased:
        menuView->setMouseReleased(true);

        // check breakpoint text input collision with the mouse
        x = breakpointInput->getPosX();
        y = breakpointInput->getPosY();
        w = breakpointInput->getWidth();
        h = breakpointInput->getHeight();
        breakpointInput->setActive((mousePos.x >= x && mousePos.x <= x + w && mousePos.y >= y && mousePos.y <= y + h));

        // check scroll text input collision with the mouse
        x = scrollInput->getPosX();
        y = scrollInput->getPosY();
        w = scrollInput->getWidth();
        h = scrollInput->getHeight();
        scrollInput->setActive((mousePos.x >= x && mousePos.x <= x + w && mousePos.y >= y && mousePos.y <= y + h));
        break;
    case sf::Event::MouseWheelScrolled:
        scrollAdr -= floor(event.mouseWheelScroll.delta * (fps / 2)) * 4;
        break;
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape)
        {
            run = false;
        }
        else if (event.key.code == sf::Keyboard::F1)
        {
            cpuHasPaused = false;
            if (cpu)
            {
                cpu->pause();
            }
        }
        else if (event.key.code == sf::Keyboard::F2)
        {
            cpu->step();
        }
        else if (event.key.code == sf::Keyboard::F3)
        {
            hexDisplay = !hexDisplay;
        }
        else if (event.key.code == sf::Keyboard::F4)
        {
            cpu->step();
            cpuHasPaused = false;
        }
        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (breakpointInput->isActive())
            {
                breakpointInput->setActive(false);
                std::string strAdr = breakpointInput->getInput();
                try
                {
                    uint16_t adr = std::stoi(strAdr, NULL, 16);
                    std::vector<uint16_t> breakpoints = cpu->getBreakpoints();
                    if (std::count(breakpoints.begin(), breakpoints.end(), adr))
                    {
                        cpu->removeBreakpoint(adr);
                    }
                    else
                    {
                        cpu->setBreakpoint(adr);
                    }
                }
                catch (const std::exception &e)
                {
                    printError(strAdr + " is not a valid hex adress");
                }
            }
            if (scrollInput->isActive())
            {
                scrollInput->setActive(false);
                std::string strAdr = scrollInput->getInput();
                try
                {
                    uint16_t adr = std::stoi(strAdr, NULL, 16);
                    scrollAdr = adr;
                }
                catch (const std::exception &e)
                {
                    printError(strAdr + " is not a valid hex adress");
                }
            }
        }
        break;
    case sf::Event::TextEntered:
        breakpointInput->inputChar(event.text.unicode);
        scrollInput->inputChar(event.text.unicode);
        break;
    default:
        break;
    }
}
