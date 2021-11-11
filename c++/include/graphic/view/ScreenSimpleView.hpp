#ifndef SCREENSIMPLEVIEW_HPP
#define SCREENSIMPLEVIEW_HPP

#include "graphic/view/View.hpp"
#include "computer/RAM.hpp"
#include <memory>

namespace graphic::view
{
    class ScreenSimpleView: public View
    {
    private:
        std::shared_ptr<computer::RAM> ram;
        sf::Color color;
        sf::Color backgroundColor;
    public:
        ScreenSimpleView(std::shared_ptr<computer::RAM> ram);
        ~ScreenSimpleView();

        void draw(sf::RenderTexture &window);
        void setColor(sf::Color c);
        void setBackgroundColor(sf::Color c);
    };
} // namespace graphic::view


#endif // SCREENSIMPLEVIEW_HPP