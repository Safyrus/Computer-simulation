#ifndef MENUACTIONTEST_HPP
#define MENUACTIONTEST_HPP

#include "data/menu/MenuAction.hpp"
#include <string>

namespace data::menu
{
    class MenuActionTest : public MenuAction
    {
    private:
        std::string test;

    public:
        MenuActionTest(std::string test);
        ~MenuActionTest();

        void execute();
    };
}

#endif // MENUACTIONTEST_HPP