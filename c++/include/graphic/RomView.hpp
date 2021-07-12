#ifndef ROMVIEW_HPP
#define ROMVIEW_HPP

#include "graphic/View.hpp"
#include "computer/ROM.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

namespace graphic
{
    class RomView : public View
    {
    private:
        std::shared_ptr<computer::ROM> rom;
        sf::Font font;
        uint8_t page;

    public:
        RomView(std::shared_ptr<computer::ROM> rom);
        ~RomView();

        void draw(sf::RenderWindow &window);
        void setPage(uint8_t page);
        uint8_t getPage();
    };
} // namespace graphic

#endif // ROMVIEW_HPP