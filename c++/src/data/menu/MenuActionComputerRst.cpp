#include "data/menu/MenuActionComputerRst.hpp"
#include "utils/console.hpp"

data::menu::MenuActionComputerRst::MenuActionComputerRst(std::shared_ptr<computer::Computer> com): MenuAction(false)
{
    printDebug("MenuActionComputerRst: Creation");
    this->com = com;
}

data::menu::MenuActionComputerRst::~MenuActionComputerRst()
{
}

void data::menu::MenuActionComputerRst::execute()
{
    com->reset();
}
