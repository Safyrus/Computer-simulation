#ifndef IOCONTROLLERWINDOW_HPP
#define IOCONTROLLERWINDOW_HPP

#include "graphic/window/Window.hpp"
#include "graphic/view/IOControllerView.hpp"

#include "computer/IOController.hpp"

namespace graphic::window
{
    class IOControllerWindow : public Window
    {
    protected:
        std::shared_ptr<computer::IOController> ioCtrl;
        std::shared_ptr<view::IOControllerView> view;
        sf::RectangleShape rect;

        void start();
        void stop();
        void loop();

    public:
        IOControllerWindow(std::shared_ptr<computer::IOController> rom);
        IOControllerWindow(std::shared_ptr<computer::IOController> rom, std::string windowName);
        IOControllerWindow(std::shared_ptr<computer::IOController> rom, std::string windowName, bool debug);
        ~IOControllerWindow();
    };
} // namespace graphic::window

#endif // IOCONTROLLERWINDOW_HPP