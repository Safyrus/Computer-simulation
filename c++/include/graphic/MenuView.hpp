#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "data/menu/Menu.hpp"
#include "SFML/Graphics.hpp"

namespace graphic
{
    class MenuView
    {
    private:
        std::shared_ptr<data::menu::Menu> menu;
        sf::Font font;
        int x, y;
        int w, h;
        int scale;
        int mx, my;
        bool pressed;
        bool released;

    public:
        MenuView(std::shared_ptr<data::menu::Menu> menu);
        ~MenuView();

        void setPos(int x, int y);
        void setSize(int w, int h);
        void setScale(float s);
        void setMousePos(int x, int y);
        void setMousePressed(bool pressed);
        void setMouseReleased(bool released);
        void draw(sf::RenderWindow &window);
    };
} // namespace graphic


#endif // MENUVIEW_HPP