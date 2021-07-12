#ifndef IOCONTROLLERWINDOW_HPP
#define IOCONTROLLERWINDOW_HPP

#include "graphic/Window.hpp"
#include "graphic/IOControllerView.hpp"

#include "computer/IOController.hpp"

namespace graphic
{
    class IOControllerWindow : public Window
    {
    protected:
        std::shared_ptr<computer::IOController> ioCtrl;
        std::shared_ptr<IOControllerView> view;
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
} // namespace graphic

#endif // IOCONTROLLERWINDOW_HPP