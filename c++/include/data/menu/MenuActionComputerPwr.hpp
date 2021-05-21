#ifndef MENUACTIONCOMPUTERPWR_HPP
#define MENUACTIONCOMPUTERPWR_HPP

#include "data/menu/MenuAction.hpp"
#include "computer/Computer.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionComputerPwr : public MenuAction
    {
    private:
        std::shared_ptr<computer::Computer> com;
    public:
        MenuActionComputerPwr(std::shared_ptr<computer::Computer> com);
        ~MenuActionComputerPwr();

        void execute();
    };
}

#endif // MENUACTIONCOMPUTERPWR_HPP