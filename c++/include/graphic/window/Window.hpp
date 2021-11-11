#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>

#include <SFML/Graphics.hpp>

namespace graphic::window
{
    class Window
    {
    private:
        sf::RenderWindow window;
        void doEventFirstSubWindow(sf::Event &event);
        const sf::Texture &getWindowTexture();
        int getFirstSubWindowWidth();
        int getFirstSubWindowHeight();
        void setFirstSubWindowMousePos(sf::Vector2f mousePos);
        bool doResize = false;
        bool getDoResize();
        void setDoResize(bool doResize);
        bool mainWindow;

    protected:
        std::vector<std::shared_ptr<graphic::window::Window>> subWindows;
        sf::RenderTexture windowTexture;
        std::string windowName;
        sf::Vector2f mousePos;
        int width;
        int height;
        int fps;

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void loop() = 0;
        virtual void doEvent(sf::Event &event);

        void addSubWindow(std::shared_ptr<graphic::window::Window> subWindow);
        std::vector<std::shared_ptr<graphic::window::Window>> findSubWinByName(std::string name);
        void printDebug(std::string msg);
        void printError(std::string msg);
        sf::View fixRatioCenterView();
        void loopSubWindows();
        void loopFirstSubWindow();
        void stopSubWindows();
        void createRenderingWindow();
        void closeRenderingWindow();

        bool oneWindowMode;
        bool fullscreen;

    public:
        bool run;
        bool debug = true;

        Window();
        Window(std::string windowName, bool debug = false, bool oneWindowMode = false);
        virtual ~Window();

        void display();
        virtual void openSubWindow(std::string windowName);
    };
} // namespace graphic::window

#endif // WINDOW_HPP