#ifndef CPUWINDOW_HPP
#define CPUWINDOW_HPP

#include "graphic/window/Window.hpp"

#include "computer/CPU.hpp"

namespace graphic::window
{
    class CPUWindow : public Window
    {
    protected:
        std::shared_ptr<computer::CPU> cpu;
        sf::Font font;
        sf::Text text;
        sf::RectangleShape rect;
        sf::Texture board;

        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();
        void doEvent(sf::Event &event);

    public:
        CPUWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName = "S257 Dynamic Recompiler - CPU Window", bool debug = false);
        ~CPUWindow();
    };
} // namespace graphic::window

#endif // CPUWINDOW_HPP