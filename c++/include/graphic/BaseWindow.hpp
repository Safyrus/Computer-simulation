#ifndef BASEWINDOW_HPP
#define BASEWINDOW_HPP

#include "graphic/Window.hpp"

namespace graphic
{
    class BaseWindow : public Window
    {
    protected:
        void start();
        void stop();
        void loop();
    public:
        BaseWindow();
        ~BaseWindow();
    };
} // namespace graphic

#endif // BASEWINDOW_HPP