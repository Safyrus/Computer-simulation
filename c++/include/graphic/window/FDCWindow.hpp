#ifndef FDCWINDOW_HPP
#define FDCWINDOW_HPP

#include "graphic/window/Window.hpp"

#include "computer/FDC.hpp"

namespace graphic::window
{
    class FDCWindow : public Window
    {
    protected:
        std::shared_ptr<computer::FDC> fdc;
        sf::Font font;
        sf::Text txt;
        sf::RectangleShape rect;
        sf::Texture board;
        sf::Texture led_green_on;
        sf::Texture led_red_on;

        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();

    public:
        FDCWindow(std::shared_ptr<computer::FDC> fdc, std::string windowName = "S257 Dynamic Recompiler - FDC Window", bool debug = false);
        ~FDCWindow();
    };
} // namespace graphic::window

#endif // FDCWINDOW_HPP