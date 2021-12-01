#ifndef VPUWINDOW_HPP
#define VPUWINDOW_HPP

#include "graphic/window/Window.hpp"

#include "computer/VPU.hpp"

namespace graphic::window
{
    class VPUWindow : public Window
    {
    protected:
        std::shared_ptr<computer::VPU> vpu;
        sf::Font font;
        sf::Text statesTxt;
        sf::Text modeTxt;
        sf::RectangleShape rect;
        sf::Texture board;
        sf::Texture led_green_on;
        sf::Texture led_red_on;

        void openTexture(sf::Texture &texture, std::string file);

        void start();
        void stop();
        void loop();
        void doEvent(sf::Event &event);

    public:
        VPUWindow(std::shared_ptr<computer::VPU> vpu);
        VPUWindow(std::shared_ptr<computer::VPU> vpu, std::string windowName);
        VPUWindow(std::shared_ptr<computer::VPU> vpu, std::string windowName, bool debug);
        ~VPUWindow();
    };
} // namespace graphic::window

#endif // VPUWINDOW_HPP