#include "data/menu/MenuActionLoadConfig.hpp"
#include "utils/console.hpp"

#include <iostream>

data::menu::MenuActionLoadConfig::MenuActionLoadConfig(std::shared_ptr<graphic::window::MainWindow> win, std::string file): MenuAction(false)
{
    printDebug("MenuActionOpenWindow: Creation");
    this->win = win;
    this->file = file;
}

data::menu::MenuActionLoadConfig::~MenuActionLoadConfig()
{

}

void data::menu::MenuActionLoadConfig::execute()
{
    win->loadConfigNxtFrame(file);
}
