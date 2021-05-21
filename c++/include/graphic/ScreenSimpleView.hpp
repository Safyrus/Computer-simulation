#ifndef SCREENSIMPLEVIEW_HPP
#define SCREENSIMPLEVIEW_HPP

#include "graphic/View.hpp"
#include "computer/RAM.hpp"
#include <memory>

namespace graphic
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

        void draw(sf::RenderWindow &window);
        void setColor(sf::Color c);
        void setBackgroundColor(sf::Color c);
    };
} // namespace graphic


#endif // SCREENSIMPLEVIEW_HPP