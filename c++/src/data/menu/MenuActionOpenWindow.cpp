#include "data/menu/MenuActionOpenWindow.hpp"
#include "utils/console.hpp"

#include <iostream>

data::menu::MenuActionOpenWindow::MenuActionOpenWindow(std::shared_ptr<graphic::window::Window> win, std::string name): MenuAction(false)
{
    printDebug("MenuActionOpenWindow: Creation");
    this->win = win;
    this->name = name;
}

data::menu::MenuActionOpenWindow::~MenuActionOpenWindow()
{

}

void data::menu::MenuActionOpenWindow::execute()
{
    win->openSubWindow(name);
}
