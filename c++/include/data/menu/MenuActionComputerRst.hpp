#ifndef MENUACTIONCOMPUTERRST_HPP
#define MENUACTIONCOMPUTERRST_HPP

#include "data/menu/MenuAction.hpp"
#include "computer/Computer.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionComputerRst : public MenuAction
    {
    private:
        std::shared_ptr<computer::Computer> com;
    public:
        MenuActionComputerRst(std::shared_ptr<computer::Computer> com);
        ~MenuActionComputerRst();

        void execute();
    };
}

#endif // MENUACTIONCOMPUTERRST_HPP