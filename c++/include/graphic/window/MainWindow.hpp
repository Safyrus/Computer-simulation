#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include "computer/Computer.hpp"
#include "computer/Keyboard.hpp"
#include "graphic/window/Window.hpp"
#include "graphic/view/MenuView.hpp"
#include "graphic/view/ScreenVPUView.hpp"
#include "data/menu/Menu.hpp"

namespace graphic::window
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
        std::shared_ptr<graphic::view::MenuView> menuView;
        void makeMenu();
        bool showMenu;
        bool loadConf;
        std::string conf;

        std::shared_ptr<graphic::view::ScreenVPUView> screenVpu;
        std::shared_ptr<computer::Keyboard> keyboard;

        void start();
        void stop();
        void loop();

    public:
        MainWindow();
        MainWindow(std::string windowName, bool debug = false, std::string prog = "prog/verifCPU/verifCPU", uint32_t hz = 0, bool printCPU = true);
        ~MainWindow();

        void openSubWindow(std::string windowName);

        void loadConfig(std::string filePath);
        void loadConfigNxtFrame(std::string filePath);
        /*void saveConfig(std::string filePath);

        void loadState(std::string filePath);
        void saveState(std::string filePath);*/
    };
} // namespace graphic::window

#endif // MAINWINDOW_HPP