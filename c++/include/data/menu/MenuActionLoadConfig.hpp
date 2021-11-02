#ifndef MENUACTIONLOADCONFIG_HPP
#define MENUACTIONLOADCONFIG_HPP

#include "data/menu/MenuAction.hpp"
#include "graphic/window/MainWindow.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionLoadConfig : public MenuAction
    {
    private:
        std::shared_ptr<graphic::window::MainWindow> win;
        std::string file;
    public:
        MenuActionLoadConfig(std::shared_ptr<graphic::window::MainWindow> win, std::string file);
        ~MenuActionLoadConfig();

        void execute();
    };
}

#endif // MENUACTIONLOADCONFIG_HPP