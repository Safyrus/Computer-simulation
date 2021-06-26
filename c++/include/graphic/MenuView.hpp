#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "data/menu/Menu.hpp"
#include "SFML/Graphics.hpp"
#include "graphic/View.hpp"

namespace graphic
{
    class MenuView : public View
    {
    private:
        std::shared_ptr<data::menu::Menu> menu;
        sf::Font font;
        int scale;
        bool vertical;
        std::shared_ptr<MenuView> subMenu;

        void drawHorinzontal(sf::RenderWindow &window);
        void drawVertical(sf::RenderWindow &window);

    public:
        MenuView(std::shared_ptr<data::menu::Menu> menu);
        MenuView(std::shared_ptr<data::menu::Menu> menu, bool vertical);
        ~MenuView();

        void setMousePos(int x, int y);
        void setMousePressed(bool pressed);
        void setMouseReleased(bool released);

        void draw(sf::RenderWindow &window);
        void setScale(int scale);
    };
} // namespace graphic

#endif // MENUVIEW_HPP