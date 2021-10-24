#include "data/menu/MenuActionComputerPwr.hpp"
#include "utils/console.hpp"

data::menu::MenuActionComputerPwr::MenuActionComputerPwr(std::shared_ptr<computer::Computer> com): MenuAction(false)
{
    printDebug("MenuActionComputerPwr: Creation");
    this->com = com;
}

data::menu::MenuActionComputerPwr::~MenuActionComputerPwr()
{
}

void data::menu::MenuActionComputerPwr::execute()
{
    com->power();
}
