#include "graphic/view/ScreenVPUView.hpp"

graphic::view::ScreenVPUView::ScreenVPUView(std::shared_ptr<computer::VPU> vpu)
{
    this->vpu = vpu;
    w = 256;
    h = 256;
    filterColor = sf::Color::White;
}

graphic::view::ScreenVPUView::~ScreenVPUView()
{

}

void graphic::view::ScreenVPUView::draw(sf::RenderTexture &window)
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

void graphic::view::ScreenVPUView::setFilterColor(sf::Color c)
{
    filterColor = c;
}
