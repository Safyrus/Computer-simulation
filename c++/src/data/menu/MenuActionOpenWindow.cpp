#include "data/menu/MenuActionOpenWindow.hpp"

#include <iostream>

data::menu::MenuActionOpenWindow::MenuActionOpenWindow(std::shared_ptr<graphic::Window> win, std::string name)
{
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
