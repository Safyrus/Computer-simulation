#ifndef MENUACTION_HPP
#define MENUACTION_HPP

namespace data::menu
{
    class MenuAction
    {
    private:
    public:
        MenuAction();
        ~MenuAction();

        virtual void execute() = 0;
    };
}

#endif // MENUACTION_HPP