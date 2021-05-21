#include "data/menu/MenuActionDoubleCPUHz.hpp"

data::menu::MenuActionDoubleCPUHz::MenuActionDoubleCPUHz(std::shared_ptr<computer::CPU> cpu)
{
    this->cpu = cpu;
}

data::menu::MenuActionDoubleCPUHz::~MenuActionDoubleCPUHz()
{
}

void data::menu::MenuActionDoubleCPUHz::execute()
{
    cpu->hz *= 2;
}
