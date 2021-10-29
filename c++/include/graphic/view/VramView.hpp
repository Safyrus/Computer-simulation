#ifndef VRAMVIEW_HPP
#define VRAMVIEW_HPP

#include "graphic/view/View.hpp"
#include "computer/VRAM.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

namespace graphic::view
{
    class VramView : public View
    {
    private:
        std::shared_ptr<computer::VRAM> vram;
        sf::Font font;
        uint8_t page;

    public:
        VramView(std::shared_ptr<computer::VRAM> vram);
        ~VramView();

        void draw(sf::RenderWindow &window);
        void setPage(uint8_t page);
        uint8_t getPage();
    };
} // namespace graphic::view

#endif // VRAMVIEW_HPP