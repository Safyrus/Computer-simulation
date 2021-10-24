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

        void start();
        void stop();
        void loop();

    public:
        CPUWindow(std::shared_ptr<computer::CPU> cpu);
        CPUWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName);
        CPUWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName, bool debug);
        ~CPUWindow();
    };
} // namespace graphic::window

#endif // CPUWINDOW_HPP