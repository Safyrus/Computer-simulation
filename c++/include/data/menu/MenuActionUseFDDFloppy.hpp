#ifndef MENUACTIONUSEFDDFLOPPY_HPP
#define MENUACTIONUSEFDDFLOPPY_HPP

#include "data/menu/MenuAction.hpp"
#include "computer/FDD.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionUseFDDFloppy : public MenuAction
    {
    private:
        std::shared_ptr<computer::FDD> fdd;
    public:
        MenuActionUseFDDFloppy(std::shared_ptr<computer::FDD> fdd);
        ~MenuActionUseFDDFloppy();

        void execute();
    };
}

#endif // MENUACTIONUSEFDDFLOPPY_HPP