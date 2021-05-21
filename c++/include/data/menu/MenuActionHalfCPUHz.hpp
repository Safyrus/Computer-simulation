#ifndef MENUACTIONHALFCPUHZ_HPP
#define MENUACTIONHALFCPUHZ_HPP

#include "data/menu/MenuAction.hpp"
#include "computer/CPU.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionHalfCPUHz : public MenuAction
    {
    private:
        std::shared_ptr<computer::CPU> cpu;
    public:
        MenuActionHalfCPUHz(std::shared_ptr<computer::CPU> cpu);
        ~MenuActionHalfCPUHz();

        void execute();
    };
}

#endif // MENUACTIONHALFCPUHZ_HPP