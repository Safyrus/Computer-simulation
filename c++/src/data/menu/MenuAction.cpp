#include "data/menu/MenuAction.hpp"
#include "utils/console.hpp"

data::menu::MenuAction::MenuAction(bool print)
{
    if(print)
    {
        printDebug("MenuAction: Creation");
    }
}

data::menu::MenuAction::~MenuAction()
{
    printDebug("MenuAction: Destruction");
}
