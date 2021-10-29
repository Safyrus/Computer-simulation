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
        sf::Font font;
        sf::Text text;
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
        uint32_t lastDeviceNumer;

        bool cursorOnPWR;
        bool cursorOnRST;
        bool mousePressed;

        std::shared_ptr<data::menu::Menu> menu;
        std::shared_ptr<view::MenuView> menuView;
        void makeMenu();
        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();

    public:
        ComputerWindow(std::shared_ptr<computer::Computer> computer);
        ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName);
        ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName, bool debug);
        ~ComputerWindow();

        void openSubWindow(std::string windowName);
    };
} // namespace graphic::window

#endif // COMPUTERWINDOW_HPP