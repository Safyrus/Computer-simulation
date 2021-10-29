#ifndef VRAMWINDOW_HPP
#define VRAMWINDOW_HPP

#include "graphic/window/Window.hpp"
#include "graphic/view/VRamView.hpp"

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

    public:
        VramWindow(std::shared_ptr<computer::VRAM> vram);
        VramWindow(std::shared_ptr<computer::VRAM> vram, std::string windowName);
        VramWindow(std::shared_ptr<computer::VRAM> vram, std::string windowName, bool debug);
        ~VramWindow();
    };
} // namespace graphic::window

#endif // VRAMWINDOW_HPP