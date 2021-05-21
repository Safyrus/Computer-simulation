#include "data/menu/MenuActionHalfCPUHz.hpp"

data::menu::MenuActionHalfCPUHz::MenuActionHalfCPUHz(std::shared_ptr<computer::CPU> cpu)
{
    this->cpu = cpu;
}

data::menu::MenuActionHalfCPUHz::~MenuActionHalfCPUHz()
{
}

void data::menu::MenuActionHalfCPUHz::execute()
{
    if(cpu->hz / 2 > 0)
    {
        cpu->hz /= 2;
    }
}
