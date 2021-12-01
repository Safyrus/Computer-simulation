#include "data/menu/MenuActionCPUPause.hpp"
#include "utils/console.hpp"

data::menu::MenuActionCPUPause::MenuActionCPUPause(std::shared_ptr<computer::CPU> cpu): MenuAction(false)
{
    printDebug("MenuActionCPUPause: Creation");
    this->cpu = cpu;
}

data::menu::MenuActionCPUPause::~MenuActionCPUPause()
{
}

void data::menu::MenuActionCPUPause::execute()
{
    cpu->pause();
}
