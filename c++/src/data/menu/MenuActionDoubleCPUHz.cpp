#include "data/menu/MenuActionDoubleCPUHz.hpp"
#include "utils/console.hpp"

data::menu::MenuActionDoubleCPUHz::MenuActionDoubleCPUHz(std::shared_ptr<computer::CPU> cpu): MenuAction(false)
{
    printDebug("MenuActionDoubleCPUHz: Creation");
    this->cpu = cpu;
}

data::menu::MenuActionDoubleCPUHz::~MenuActionDoubleCPUHz()
{
}

void data::menu::MenuActionDoubleCPUHz::execute()
{
    cpu->hz *= 2;
}
