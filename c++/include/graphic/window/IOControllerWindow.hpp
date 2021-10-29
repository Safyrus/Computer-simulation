#ifndef IOCONTROLLERWINDOW_HPP
#define IOCONTROLLERWINDOW_HPP

#include "graphic/window/Window.hpp"

#include "computer/IOController.hpp"

namespace graphic::window
{
    class IOControllerWindow : public Window
    {
    protected:
        std::shared_ptr<computer::IOController> ioCtrl;
        sf::RectangleShape rect;
        sf::Font font;
        sf::Text txt;
        sf::Texture board;
        sf::Texture led_red_on;

        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();

    public:
        IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl);
        IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl, std::string windowName);
        IOControllerWindow(std::shared_ptr<computer::IOController> ioCtrl, std::string windowName, bool debug);
        ~IOControllerWindow();
    };
} // namespace graphic::window

#endif // IOCONTROLLERWINDOW_HPP