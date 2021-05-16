#include "data/menu/Menu.hpp"

data::menu::Menu::Menu()
{

}

data::menu::Menu::~Menu()
{

}

void data::menu::Menu::execute()
{
    actions[selected].get()->execute();
}

void data::menu::Menu::select(int index)
{
    if(index < 0)
        index = 0;
    if(index >= (int)names.size())
        index = names.size()-1;
    selected = index;
}

void data::menu::Menu::select(std::string name)
{
    for (unsigned int i = 0; i < names.size(); i++)
    {
        if(names[i].compare(name) == 0)
        {
            selected = i;
            return;
        }
    }
}

int data::menu::Menu::getIndex()
{
    return selected;
}

std::string data::menu::Menu::getName(int index)
{
    if(index < 0)
        index = 0;
    if(index >= (int)names.size())
        index = names.size()-1;
    return names[index];
}

std::shared_ptr<data::menu::MenuAction> data::menu::Menu::getAction(int index)
{
    if(index < 0)
        index = 0;
    if(index >= (int)names.size())
        index = names.size()-1;
    return actions[index];
}

int data::menu::Menu::size()
{
    return names.size();
}

void data::menu::Menu::addItem(std::string name, std::shared_ptr<MenuAction> action)
{
    names.push_back(name);
    actions.push_back(action);
}

