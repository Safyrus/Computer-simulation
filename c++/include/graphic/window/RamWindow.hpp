#ifndef RAMWINDOW_HPP
#define RAMWINDOW_HPP

#include "graphic/window/Window.hpp"
#include "graphic/view/RamView.hpp"

#include "computer/RAM.hpp"

namespace graphic::window
{
    class RamWindow : public Window
    {
    protected:
        std::shared_ptr<computer::RAM> ram;
        std::shared_ptr<view::RamView> ramView;
        sf::RectangleShape rect;

        void start();
        void stop();
        void loop();

    public:
        RamWindow(std::shared_ptr<computer::RAM> ram);
        RamWindow(std::shared_ptr<computer::RAM> ram, std::string windowName);
        RamWindow(std::shared_ptr<computer::RAM> ram, std::string windowName, bool debug);
        ~RamWindow();
    };
} // namespace graphic::window

#endif // RAMWINDOW_HPP