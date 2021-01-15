#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include "computer/Computer.hpp"
#include "graphic/Window.hpp"

namespace graphic
{
    class MainWindow : public Window
    {
    protected:
        std::shared_ptr<computer::Computer> computer;
        std::string computerWindowName;
        sf::Font font;
        sf::Text text;
        sf::RectangleShape rect;

        void start();
        void stop();
        void loop();

    public:
        MainWindow();
        MainWindow(std::string windowName);
        MainWindow(std::string windowName, bool debug);
        ~MainWindow();

        /*void loadConfig(std::string filePath);
        void saveConfig(std::string filePath);

        void loadState(std::string filePath);
        void saveState(std::string filePath);*/
    };
} // namespace graphic

#endif // MAINWINDOW_HPP