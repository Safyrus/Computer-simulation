#ifndef MENUACTIONUSEFDDLOCK_HPP
#define MENUACTIONUSEFDDLOCK_HPP

#include "data/menu/MenuAction.hpp"
#include "computer/FDD.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionUseFDDLock : public MenuAction
    {
    private:
        std::shared_ptr<computer::FDD> fdd;
    public:
        MenuActionUseFDDLock(std::shared_ptr<computer::FDD> fdd);
        ~MenuActionUseFDDLock();

        void execute();
    };
}

#endif // MENUACTIONUSEFDDLOCK_HPP