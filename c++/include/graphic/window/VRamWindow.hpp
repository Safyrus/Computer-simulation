#ifndef VRAMWINDOW_HPP
#define VRAMWINDOW_HPP

#include "graphic/window/Window.hpp"
#include "graphic/view/VramView.hpp"

#include "computer/VRAM.hpp"

namespace graphic::window
{
    class VramWindow : public Window
    {
    protected:
        std::shared_ptr<computer::VRAM> vram;
        std::shared_ptr<view::VramView> vramView;
        sf::RectangleShape rect;

        void start();
        void stop();
        void loop();
        void doEvent(sf::Event &event);

    public:
        VramWindow(std::shared_ptr<computer::VRAM> vram);
        VramWindow(std::shared_ptr<computer::VRAM> vram, std::string windowName, bool debug = false);
        ~VramWindow();
    };
} // namespace graphic::window

#endif // VRAMWINDOW_HPP