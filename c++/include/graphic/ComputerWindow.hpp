#ifndef COMPUTERWINDOW_HPP
#define COMPUTERWINDOW_HPP

#include "graphic/Window.hpp"

#include "computer/Computer.hpp"

namespace graphic
{
    class ComputerWindow : public Window
    {
    protected:
        std::shared_ptr<computer::Computer> computer;
        sf::Font font;
        sf::Text text;
        sf::RectangleShape rect;
        
        void start();
        void stop();
        void loop();
    public:
        ComputerWindow(std::shared_ptr<computer::Computer> computer);
        ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName);
        ComputerWindow(std::shared_ptr<computer::Computer> computer, std::string windowName, bool debug);
        ~ComputerWindow();
    };
} // namespace graphic

#endif // COMPUTERWINDOW_HPP