#ifndef HWSTATESWINDOW_HPP
#define HWSTATESWINDOW_HPP

#include "graphic/window/Window.hpp"

#include "computer/HardwareStates.hpp"

namespace graphic::window
{
    class HwStatesWindow : public Window
    {
    protected:
        std::shared_ptr<computer::HardwareStates> hwstats;
        sf::Font font;
        sf::Text timerTxt;
        sf::Text buzzerTxt;
        sf::RectangleShape rect;
        sf::Texture board;
        sf::Texture led_green_on;
        sf::Texture led_red_on;

        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();

    public:
        HwStatesWindow(std::shared_ptr<computer::HardwareStates> hwstats);
        HwStatesWindow(std::shared_ptr<computer::HardwareStates> hwstats, std::string windowName);
        HwStatesWindow(std::shared_ptr<computer::HardwareStates> hwstats, std::string windowName, bool debug);
        ~HwStatesWindow();
    };
} // namespace graphic::window

#endif // HWSTATESWINDOW_HPP