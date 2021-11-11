#include "data/menu/MenuActionUseFDDFloppy.hpp"
#include "utils/console.hpp"

data::menu::MenuActionUseFDDFloppy::MenuActionUseFDDFloppy(std::shared_ptr<computer::FDD> fdd): MenuAction(false)
{
    printDebug("MenuActionUseFDDFloppy: Creation");
    this->fdd = fdd;
}

data::menu::MenuActionUseFDDFloppy::~MenuActionUseFDDFloppy()
{
}

void data::menu::MenuActionUseFDDFloppy::execute()
{
    if(fdd)
    {
        if(fdd->isFloppyIn())
        {
            fdd->eject();
        }else
        {
            std::shared_ptr<data::Floppy> floppy = std::make_shared<data::Floppy>();
            floppy->load("floppy.img");
            fdd->insert(floppy);
        }
    }
}
