#ifndef COMPUTERWINDOW_HPP
#define COMPUTERWINDOW_HPP

#include "computer/Computer.hpp"
#include "graphic/window/Window.hpp"
#include "graphic/view/MenuView.hpp"
#include "data/menu/Menu.hpp"

namespace graphic::window
{
    class ComputerWindow : public Window, public std::enable_shared_from_this<ComputerWindow>
    {
    protected:
        std::shared_ptr<computer::Computer> computer;
        std::string cpuWindowName;
        std::string debugerWindowName;
        sf::RectangleShape rect;
        sf::Texture com;
        sf::Texture pwrOn;
        sf::Texture pwrOff;
        sf::Texture buttonOn;
        sf::Texture buttonOff;
        sf::Texture dskOn;
        sf::Texture dskOff;
        sf::Texture floppyOn;
        sf::Texture floppyOff;
        sf::Texture floppyDsk;
        uint32_t lastDeviceNumer;

        bool cursorOnPWR;
        bool cursorOnRST;
        bool cursorOnLock;
        bool cursorOnFloppy;
        bool mousePressed;

        std::shared_ptr<data::menu::Menu> menu;
        std::shared_ptr<view::MenuView> menuView;
        void makeMenu();
        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();
        void doEvent(sf::Event &event);

    public:
        ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName = "S257 Dynamic Recompiler - Computer Window", bool debug = false, bool oneWindow = false);
        ~ComputerWindow();

        void openSubWindow(std::string windowName);
    };
} // namespace graphic::window

#endif // COMPUTERWINDOW_HPP