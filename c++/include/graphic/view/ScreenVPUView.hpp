#ifndef SCREENVPUVIEW_HPP
#define SCREENVPUVIEW_HPP

#include "graphic/view/View.hpp"
#include "computer/VPU.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

namespace graphic::view
{
    class ScreenVPUView: public View
    {
    private:
        std::shared_ptr<computer::VPU> vpu;
        sf::Color filterColor;
    public:
        ScreenVPUView(std::shared_ptr<computer::VPU> vpu);
        ~ScreenVPUView();

        void draw(sf::RenderTexture &window);
        void setFilterColor(sf::Color c);
    };
} // namespace graphic::view


#endif // SCREENVPUVIEW_HPP