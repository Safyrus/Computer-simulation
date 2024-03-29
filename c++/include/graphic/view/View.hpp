#ifndef VIEW_HPP
#define VIEW_HPP

#include "SFML/Graphics.hpp"

namespace graphic::view
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
        int getPosX();
        int getPosY();
        int getWidth();
        int getHeight();
        virtual void draw(sf::RenderTexture &window) = 0;
    };
} // namespace graphic::view


#endif // VIEW_HPP