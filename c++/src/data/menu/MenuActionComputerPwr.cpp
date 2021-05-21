#include "data/menu/MenuActionComputerPwr.hpp"

data::menu::MenuActionComputerPwr::MenuActionComputerPwr(std::shared_ptr<computer::Computer> com)
{
    this->com = com;
}

data::menu::MenuActionComputerPwr::~MenuActionComputerPwr()
{
}

void data::menu::MenuActionComputerPwr::execute()
{
    com->power();
}
