#include "graphic/window/VPUWindow.hpp"

#include <sstream>
#include <iomanip>

graphic::window::VPUWindow::VPUWindow(std::shared_ptr<computer::VPU> vpu)
{
    this->vpu = vpu;
    windowName = "S257 Dynamic Recompiler - VPU Window";
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::VPUWindow::VPUWindow(std::shared_ptr<computer::VPU> vpu, std::string windowName)
{
    this->vpu = vpu;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::VPUWindow::VPUWindow(std::shared_ptr<computer::VPU> vpu, std::string windowName, bool debug)
{
    this->debug = debug;
    this->vpu = vpu;
    this->windowName = windowName;
    width = 128;
    height = 128;
    printDebug("Creation");
}

graphic::window::VPUWindow::~VPUWindow()
{
}

void graphic::window::VPUWindow::openTexture(sf::Texture &texture, std::string file)
{
    if (!texture.loadFromFile(file))
    {
        printError("Cannot open texture " + file);
    }
    texture.setSmooth(false);
}

void graphic::window::VPUWindow::start()
{
    printDebug("Start");
    createRenderingWindow();

    // load font
    if (!font.loadFromFile("pix46.ttf"))
    {
        printError("Cannot open font");
    }
    const_cast<sf::Texture &>(font.getTexture(6)).setSmooth(false);

    // load textures
    openTexture(board, "data/img/vpu_board.png");
    openTexture(led_green_on, "data/img/vpu_led_green_on.png");
    openTexture(led_red_on, "data/img/vpu_led_red_on.png");

    // setup mode text
    modeTxt.setFont(font);
    modeTxt.setCharacterSize(6);
    modeTxt.setString("");
    modeTxt.setFillColor(sf::Color(0xf4, 0xf4, 0xf4));
    modeTxt.setPosition(sf::Vector2f(47, 48));

    // setup states text
    statesTxt.setFont(font);
    statesTxt.setCharacterSize(6);
    statesTxt.setString("");
    statesTxt.setFillColor(sf::Color(0xf4, 0xf4, 0xf4));
    statesTxt.setPosition(sf::Vector2f(47, 41));

    // setup background
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(width, height));
}

void graphic::window::VPUWindow::stop()
{
    vpu.reset();
    closeRenderingWindow();
    printDebug("Stop");
}

void graphic::window::VPUWindow::loop()
{
    // Clear screen
    windowTexture.clear(sf::Color(32, 32, 32));

    windowTexture.draw(rect);

    // draw board
    sf::Sprite boardSprite;
    boardSprite.setTexture(board);
    boardSprite.setPosition(sf::Vector2f(0, 0));
    windowTexture.draw(boardSprite);

    // draw pwr light
    sf::Sprite pwrLightSprite;
    if (vpu->getPwr())
    {
        pwrLightSprite.setTexture(led_green_on);
        pwrLightSprite.setPosition(sf::Vector2f(79, 21));
        windowTexture.draw(pwrLightSprite);
    }

    // draw vblank light
    sf::Sprite vblkSprite;
    bool vblk = (vpu->get(1) & 0x01);
    if (vblk)
    {
        vblkSprite.setTexture(led_red_on);
        vblkSprite.setPosition(sf::Vector2f(59, 25));
        windowTexture.draw(vblkSprite);
    }

    // draw states text
    uint8_t val = vpu->get(1);
    std::stringstream statesStr;
    statesStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)val;
    statesTxt.setString(statesStr.str());
    windowTexture.draw(statesTxt);

    // draw mode text
    val = vpu->get(0);
    std::stringstream modeStr;
    modeStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)val;
    modeTxt.setString(modeStr.str());
    windowTexture.draw(modeTxt);

    // Update the window
    windowTexture.display();
}

void graphic::window::VPUWindow::doEvent(sf::Event &event)
{
    Window::doEvent(event);
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape)
        {
            run = false;
        }
        break;
    default:
        break;
    }
}