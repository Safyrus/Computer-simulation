#ifndef MENUACTIONCPUSTEP_HPP
#define MENUACTIONCPUSTEP_HPP

#include "data/menu/MenuAction.hpp"
#include <memory>

#include "computer/CPU.hpp"

namespace data::menu
{
    class MenuActionCPUStep : public MenuAction
    {
    private:
        std::shared_ptr<computer::CPU> cpu;

    public:
        MenuActionCPUStep(std::shared_ptr<computer::CPU> cpu);
        ~MenuActionCPUStep();

        void execute();
    };
}

#endif // MENUACTIONCPUSTEP_HPP