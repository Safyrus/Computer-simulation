#ifndef TEXTINPUTVIEW_HPP
#define TEXTINPUTVIEW_HPP

#include "graphic/view/View.hpp"
#include "computer/VPU.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

namespace graphic::view
{
    class TextInputView: public View
    {
    private:
        sf::Font font;
        std::string input;
        bool active;
        unsigned int limit;
    public:
        TextInputView();
        ~TextInputView();

        void draw(sf::RenderTexture &window);
        std::string getInput();
        void inputChar(char c);
        void setActive(bool active);
        bool isActive();
        void setLimit(unsigned int limit);
    };
} // namespace graphic::view


#endif // TEXTINPUTVIEW_HPP