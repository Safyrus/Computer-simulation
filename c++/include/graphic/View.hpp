#ifndef VIEW_HPP
#define VIEW_HPP

#include "SFML/Graphics.hpp"

namespace graphic
{
    class View
    {
    protected:
        int x, y;
        int w, h;
        int scale;
        int mx, my;
        bool pressed;
        bool released;

    public:
        View();
        ~View();

        void setPos(int x, int y);
        void setSize(int w, int h);
        void setScale(float s);
        void setMousePos(int x, int y);
        void setMousePressed(bool pressed);
        void setMouseReleased(bool released);
        virtual void draw(sf::RenderWindow &window) = 0;
    };
} // namespace graphic


#endif // VIEW_HPP