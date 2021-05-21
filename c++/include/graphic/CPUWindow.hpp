#ifndef CPUWINDOW_HPP
#define CPUWINDOW_HPP

#include "graphic/Window.hpp"

#include "computer/Computer.hpp"

namespace graphic
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
} // namespace graphic

#endif // CPUWINDOW_HPP