#ifndef MENUACTIONCPUPAUSE_HPP
#define MENUACTIONCPUPAUSE_HPP

#include "data/menu/MenuAction.hpp"
#include <memory>

#include "computer/CPU.hpp"

namespace data::menu
{
    class MenuActionCPUPause : public MenuAction
    {
    private:
        std::shared_ptr<computer::CPU> cpu;

    public:
        MenuActionCPUPause(std::shared_ptr<computer::CPU> cpu);
        ~MenuActionCPUPause();

        void execute();
    };
}

#endif // MENUACTIONCPUPAUSE_HPP