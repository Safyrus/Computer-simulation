#include "data/menu/MenuActionCPUStep.hpp"
#include "utils/console.hpp"

data::menu::MenuActionCPUStep::MenuActionCPUStep(std::shared_ptr<computer::CPU> cpu): MenuAction(false)
{
    printDebug("MenuActionCPUStep: Creation");
    this->cpu = cpu;
}

data::menu::MenuActionCPUStep::~MenuActionCPUStep()
{
}

void data::menu::MenuActionCPUStep::execute()
{
    cpu->step();
}
