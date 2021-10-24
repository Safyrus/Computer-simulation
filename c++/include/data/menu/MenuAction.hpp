#ifndef MENUACTION_HPP
#define MENUACTION_HPP

namespace data::menu
{
    class MenuAction
    {
    private:
    public:
        MenuAction(bool print=true);
        ~MenuAction();

        virtual void execute() = 0;
    };
}

#endif // MENUACTION_HPP