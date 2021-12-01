#ifndef DEBUGERWINDOW_HPP
#define DEBUGERWINDOW_HPP

#include "graphic/window/Window.hpp"

#include "graphic/view/MenuView.hpp"
#include "graphic/view/TextInputView.hpp"

#include "computer/CPU.hpp"

#include <stdbool.h>

namespace graphic::window
{
    class DebugerWindow : public Window, public std::enable_shared_from_this<DebugerWindow>
    {
    protected:
        std::shared_ptr<computer::CPU> cpu;
        static std::map<uint8_t, std::string> CMDS;
        static std::map<uint8_t, std::string> REGNAME;
        static std::map<uint8_t, bool> DSTREG;
        static std::map<uint8_t, bool> SRCREG;
        static std::map<uint8_t, bool> VALREG;

        sf::Font font;
        sf::Text txt;
        sf::RectangleShape rect;
        std::shared_ptr<graphic::view::MenuView> menuView;
        std::shared_ptr<graphic::view::TextInputView> breakpointInput;
        std::shared_ptr<graphic::view::TextInputView> scrollInput;
        uint16_t scrollAdr;
        uint16_t lastScrollAdr;
        bool cpuHasPaused;
        bool hexDisplay;

        void start();
        void stop();
        void loop();
        void doEvent(sf::Event &event);

        void drawInstructions();
        //void writeHexLine(std::stringstream &str, uint16_t adr);
        void writeHex(std::stringstream &str, uint16_t adr);
        void writeNewLine(std::stringstream &str, uint16_t adr);
        void writeInstruction(std::stringstream &str, uint16_t adr);
        bool isInstruction(uint16_t adr);
        void drawCPUpc(int line, uint16_t adr);
        void drawBreaks(int line, uint16_t adr);

    public:
        DebugerWindow(std::shared_ptr<computer::CPU> cpu, std::string windowName = "S257 Dynamic Recompiler - Debuger Window", bool debug = false);
        ~DebugerWindow();
    };
} // namespace graphic::window

#endif // DEBUGERWINDOW_HPP