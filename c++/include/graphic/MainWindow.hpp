#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include "computer/Computer.hpp"
#include "computer/Keyboard.hpp"
#include "graphic/Window.hpp"
#include "graphic/MenuView.hpp"
#include "graphic/ScreenVPUView.hpp"
#include "data/menu/Menu.hpp"

namespace graphic
{
    class MainWindow : public Window, public std::enable_shared_from_this<MainWindow>
    {
    protected:
        std::shared_ptr<computer::Computer> computer;
        std::string computerWindowName;
        sf::Font font;
        sf::Text text;
        sf::RectangleShape rect;

        std::shared_ptr<data::menu::Menu> menu;
        std::shared_ptr<MenuView> menuView;
        void makeMenu();
        bool showMenu;

        std::shared_ptr<graphic::ScreenVPUView> screenVpu;
        std::shared_ptr<computer::Keyboard> keyboard;

        void start();
        void stop();
        void loop();

    public:
        MainWindow();
        MainWindow(std::string windowName);
        MainWindow(std::string windowName, bool debug, std::string prog);
        ~MainWindow();

        void openSubWindow(std::string windowName);

        /*void loadConfig(std::string filePath);
        void saveConfig(std::string filePath);

        void loadState(std::string filePath);
        void saveState(std::string filePath);*/
    };
} // namespace graphic

#endif // MAINWINDOW_HPP