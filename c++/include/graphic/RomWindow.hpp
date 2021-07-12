#ifndef ROMWINDOW_HPP
#define ROMWINDOW_HPP

#include "graphic/Window.hpp"
#include "graphic/RomView.hpp"

#include "computer/ROM.hpp"

namespace graphic
{
    class RomWindow : public Window
    {
    protected:
        std::shared_ptr<computer::ROM> rom;
        std::shared_ptr<RomView> romView;
        sf::RectangleShape rect;

        void start();
        void stop();
        void loop();

    public:
        RomWindow(std::shared_ptr<computer::ROM> rom);
        RomWindow(std::shared_ptr<computer::ROM> rom, std::string windowName);
        RomWindow(std::shared_ptr<computer::ROM> rom, std::string windowName, bool debug);
        ~RomWindow();
    };
} // namespace graphic

#endif // ROMWINDOW_HPP