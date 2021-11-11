#include "data/menu/MenuActionUseFDDLock.hpp"
#include "utils/console.hpp"

data::menu::MenuActionUseFDDLock::MenuActionUseFDDLock(std::shared_ptr<computer::FDD> fdd): MenuAction(false)
{
    printDebug("MenuActionUseFDDLock: Creation");
    this->fdd = fdd;
}

data::menu::MenuActionUseFDDLock::~MenuActionUseFDDLock()
{
}

void data::menu::MenuActionUseFDDLock::execute()
{
    if(fdd)
    {
        fdd->useLock(!fdd->isLock());
    }
}
