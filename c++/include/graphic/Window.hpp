#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>

#include <SFML/Graphics.hpp>

namespace graphic
{
    class Window
    {
    protected:
        std::vector<std::shared_ptr<graphic::Window>> subWindows;
        sf::RenderWindow window;
        std::string windowName;
        int width;
        int height;
        int fps;

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void loop() = 0;

        void addSubWindow(std::shared_ptr<graphic::Window> subWindow);
        std::vector<std::shared_ptr<graphic::Window>> findSubWinByName(std::string name);
        void printDebug(std::string msg);
        void printError(std::string msg);
        sf::View fixRatioCenterView();
        void loopSubWindows();
        void stopSubWindows();

    public:
        bool run;
        bool debug = true;

        Window();
        Window(std::string windowName);
        Window(std::string windowName, bool debug);
        virtual ~Window();

        void display();
        virtual void openSubWindow(std::string windowName);
    };
} // namespace graphic

#endif // WINDOW_HPP