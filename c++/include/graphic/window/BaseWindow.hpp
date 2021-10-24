#ifndef BASEWINDOW_HPP
#define BASEWINDOW_HPP

#include "graphic/window/Window.hpp"

namespace graphic::window
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
} // namespace graphic::window

#endif // BASEWINDOW_HPP