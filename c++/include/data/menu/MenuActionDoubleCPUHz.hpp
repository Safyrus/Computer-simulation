#ifndef MENUACTIONDOUBLECPUHZ_HPP
#define MENUACTIONDOUBLECPUHZ_HPP

#include "data/menu/MenuAction.hpp"
#include "computer/CPU.hpp"
#include <memory>

namespace data::menu
{
    class MenuActionDoubleCPUHz : public MenuAction
    {
    private:
        std::shared_ptr<computer::CPU> cpu;
    public:
        MenuActionDoubleCPUHz(std::shared_ptr<computer::CPU> cpu);
        ~MenuActionDoubleCPUHz();

        void execute();
    };
}

#endif // MENUACTIONDOUBLECPUHZ_HPP