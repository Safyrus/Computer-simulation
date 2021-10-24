#include "graphic/view/ScreenSimpleView.hpp"

#include <math.h>
#include <algorithm>

#include "utils/console.hpp"

graphic::view::ScreenSimpleView::ScreenSimpleView(std::shared_ptr<computer::RAM> ram)
{
    this->ram = ram;
    w = 128;
    h = 128;
    color = sf::Color(0xff, 0xff, 0xff);
    backgroundColor = sf::Color(0x00, 0x00, 0x00);
}

graphic::view::ScreenSimpleView::~ScreenSimpleView()
{

}

void graphic::view::ScreenSimpleView::draw(sf::RenderWindow &window)
{
    // create pixels array
    unsigned int pxNumber = w*h;
    uint8_t pixArray[pxNumber*4];
    for (size_t i = 0; i < pxNumber; i++)
    {
        pixArray[(i*4)+0] = backgroundColor.r;
        pixArray[(i*4)+1] = backgroundColor.g;
        pixArray[(i*4)+2] = backgroundColor.b;
        pixArray[(i*4)+3] = backgroundColor.a;
    }

    // add pixels to the array
    for (unsigned int i = 0; i < std::min((unsigned int)ram->length(), pxNumber/8); i++)
    {
        uint8_t data = ram->get(i);
        for (uint8_t j = 0; j < 8; j++)
        {
            unsigned int index = (i*8*4)+(j*4);
            bool pxActive = data&(int)(pow(2, (7-j)));
            sf::Color c = backgroundColor;
            if(pxActive)
            {
                c = color;
            }
            pixArray[index+0] = c.r;
            pixArray[index+1] = c.g;
            pixArray[index+2] = c.b;
            pixArray[index+3] = c.a;
        }
    }

    // display the screen
    sf::Image img;
    img.create(w, h, pixArray);
    sf::Sprite spr;
    sf::Texture texture;
    texture.loadFromImage(img);
    texture.setSmooth(false);
    spr.setTexture(texture);
    window.draw(spr);
}

void graphic::view::ScreenSimpleView::setColor(sf::Color c)
{
    color = c;
}

void graphic::view::ScreenSimpleView::setBackgroundColor(sf::Color c)
{
    backgroundColor = c;
}
