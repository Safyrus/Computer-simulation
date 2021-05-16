#ifndef MENUACTIONOPENWINDOW_HPP
#define MENUACTIONOPENWINDOW_HPP

#include "data/menu/MenuAction.hpp"
#include "graphic/Window.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionOpenWindow : public MenuAction
    {
    private:
        std::shared_ptr<graphic::Window> win;
        std::string name;
    public:
        MenuActionOpenWindow(std::shared_ptr<graphic::Window> win, std::string name);
        ~MenuActionOpenWindow();

        void execute();
    };
}

#endif // MENUACTIONOPENWINDOW_HPP