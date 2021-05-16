#include "data/menu/MenuActionTest.hpp"

#include <iostream>

data::menu::MenuActionTest::MenuActionTest(std::string test)
{
    this->test = test;
}

data::menu::MenuActionTest::~MenuActionTest()
{

}

void data::menu::MenuActionTest::execute()
{
    std::cout << this->test << "\n";
}
