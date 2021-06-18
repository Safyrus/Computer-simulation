#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "data/menu/Menu.hpp"
#include "SFML/Graphics.hpp"
#include "graphic/View.hpp"

namespace graphic
{
    class MenuView: public View
    {
    private:
        std::shared_ptr<data::menu::Menu> menu;
        sf::Font font;
        int scale;
    public:
        MenuView(std::shared_ptr<data::menu::Menu> menu);
        ~MenuView();
        
        void draw(sf::RenderWindow &window);
        void setScale(int scale);
    };
} // namespace graphic


#endif // MENUVIEW_HPP