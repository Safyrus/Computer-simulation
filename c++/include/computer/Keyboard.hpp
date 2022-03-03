#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "computer/IODevice.hpp"
#include <memory>

namespace computer
{
    class Keyboard : public computer::IODevice
    {
    private:
        uint8_t keyBuf[8];
        uint8_t speBuf[8];
        uint8_t bufCurs;
        uint8_t speKey;
        bool talk;
        bool talkCPU;
        bool keyLayoutMode;

        uint8_t cmdBuf[8];
        uint8_t cmdCurs;

        void keyPressed(uint8_t key, uint8_t special);
        void runStep();

    public:
        Keyboard(bool keyLayoutMode = true);
        ~Keyboard();

        void reset();
        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);

        bool getTalk(uint8_t port);
        void setTalk(uint8_t port, bool talk);
        void send(uint8_t port, uint8_t data);

        void inputEvent(sf::Event event);
    };
} // namespace computer

#endif // KEYBOARD_HPP