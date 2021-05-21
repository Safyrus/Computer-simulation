#include "graphic/CPUWindow.hpp"

#include "data/menu/MenuActionTest.hpp"

#include <sstream>
#include <iomanip>

graphic::CPUWindow::CPUWindow(std::shared_ptr<computer::CPU> cpu)
{
    this->cpu = cpu;
    windowName = "S257 Dynamic Recompiler - CPU Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::CPUWindow::CPUWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName)
{
    this->cpu = cpu;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::CPUWindow::CPUWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName, bool debug)
{
    this->cpu = cpu;
    this->windowName = windowName;
    this->debug = debug;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::CPUWindow::~CPUWindow()
{

}

void graphic::CPUWindow::start()
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

    text.setString("");
    text.setFillColor(sf::Color::White);
    sf::Vector2f pos(0, 32);
    text.setPosition(pos);

    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));

    window.setView(fixRatioCenterView());
}

void graphic::CPUWindow::stop()
{
    window.close();
    printDebug("Stop");
}

void graphic::CPUWindow::loop()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
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
        case sf::Event::KeyPressed:
            printDebug("Key " + std::to_string(event.key.code) + " Pressed");
            break;
        default:
            break;
        }
    }
    // Clear screen
    window.clear(sf::Color(32, 32, 32));

    window.draw(rect);

    // set the cpu infos string
    std::stringstream txt;
    txt << "CPU:\n";
    txt << "  HZ   : " << cpu->hz << "\n";
    txt << "  CYCLE: " << cpu->cycle << "\n";
    txt << "  ADR  : " << std::hex << std::setfill('0') << std::setw(4) << cpu->pc << "\n";
    txt << "  REG  : O  A  B  C  D  E  F  R  J1 J2 G0 G1 G2 G3 G4 G5\n         ";
    for (unsigned int i = 0; i < 16; i++)
    {
        txt << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu->reg[i] << " ";
    }

    // display the cpu infos
    text.setString(txt.str());
    window.draw(text);

    // Update the window
    window.display();
}
