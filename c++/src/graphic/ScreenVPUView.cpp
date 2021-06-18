#include "graphic/ScreenVPUView.hpp"

graphic::ScreenVPUView::ScreenVPUView(std::shared_ptr<computer::VPU> vpu)
{
    this->vpu = vpu;
    w = 256;
    h = 256;
    filterColor = sf::Color::White;
}

graphic::ScreenVPUView::~ScreenVPUView()
{

}

void graphic::ScreenVPUView::draw(sf::RenderWindow &window)
{
    // display the screen
    uint8_t* pixArray = vpu->getPixArray();
    sf::Image img;
    img.create(w, h, pixArray);
    sf::Sprite spr;
    sf::Texture texture;
    texture.loadFromImage(img);
    texture.setSmooth(false);
    spr.setTexture(texture);
    spr.setColor(filterColor);
    window.draw(spr);
}

void graphic::ScreenVPUView::setFilterColor(sf::Color c)
{
    filterColor = c;
}
