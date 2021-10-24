#include "data/menu/MenuActionTest.hpp"
#include "utils/console.hpp"

#include <iostream>

data::menu::MenuActionTest::MenuActionTest(std::string test): MenuAction(false)
{
    printDebug("MenuActionTest: Creation");
    this->test = test;
}

data::menu::MenuActionTest::~MenuActionTest()
{

}

void data::menu::MenuActionTest::execute()
{
    std::cout << this->test << "\n";
}
