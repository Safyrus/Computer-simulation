#include "data/menu/MenuActionComputerRst.hpp"

data::menu::MenuActionComputerRst::MenuActionComputerRst(std::shared_ptr<computer::Computer> com)
{
    this->com = com;
}

data::menu::MenuActionComputerRst::~MenuActionComputerRst()
{
}

void data::menu::MenuActionComputerRst::execute()
{
    com->reset();
}
