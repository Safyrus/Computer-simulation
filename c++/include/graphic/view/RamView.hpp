#ifndef RAMVIEW_HPP
#define RAMVIEW_HPP

#include "graphic/view/View.hpp"
#include "computer/RAM.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

namespace graphic::view
{
    class RamView : public View
    {
    private:
        std::shared_ptr<computer::RAM> ram;
        sf::Font font;
        uint8_t page;

    public:
        RamView(std::shared_ptr<computer::RAM> ram);
        ~RamView();

        void draw(sf::RenderWindow &window);
        void setPage(uint8_t page);
        uint8_t getPage();
    };
} // namespace graphic::view

#endif // RAMVIEW_HPP