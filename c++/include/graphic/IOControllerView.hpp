#ifndef IOCONTROLLERVIEW_HPP
#define IOCONTROLLERVIEW_HPP

#include "graphic/View.hpp"
#include "computer/IOController.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

namespace graphic
{
    class IOControllerView : public View
    {
    private:
        std::shared_ptr<computer::IOController> ioCtrl;
        sf::Font font;

    public:
        IOControllerView(std::shared_ptr<computer::IOController> ioCtrl);
        ~IOControllerView();

        void draw(sf::RenderWindow &window);
    };
} // namespace graphic

#endif // IOCONTROLLERVIEW_HPP