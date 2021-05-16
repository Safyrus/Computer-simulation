#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>
#include <memory>
#include <data/menu/MenuAction.hpp>

namespace data::menu
{
    class Menu: public MenuAction
    {
    private:
        std::vector<std::string> names;
        std::vector<std::shared_ptr<MenuAction>> actions;
        int selected;
    public:
        Menu();
        ~Menu();

        void execute();
        void select(int index);
        void select(std::string name);

        int getIndex();
        std::string getName(int index);
        std::shared_ptr<MenuAction> getAction(int index);
        int size();

        void addItem(std::string name, std::shared_ptr<MenuAction> action);
    };
}

#endif // MENU_HPP