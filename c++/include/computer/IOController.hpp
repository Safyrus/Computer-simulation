#ifndef IOCONTROLLER_HPP
#define IOCONTROLLER_HPP

#include "computer/IODevice.hpp"
#include "computer/RunnableDevice.hpp"

#include <memory>

namespace computer
{
    class IOController : public computer::IODevice, public std::enable_shared_from_this<IOController>
    {
    public:
        static const int DEVICES_SIZE = 8;
        static const int OUT_SIZE = 8;
        static const int IN_SIZE = 8;

    private:
        std::shared_ptr<IODevice> devices[DEVICES_SIZE];
        computer::RunnableDevice * runnables[DEVICES_SIZE];
        uint8_t outBuffer[DEVICES_SIZE][OUT_SIZE];
        uint8_t inBuffer[DEVICES_SIZE][IN_SIZE];
        uint8_t inBufState[DEVICES_SIZE];
        uint8_t outBufState[DEVICES_SIZE];
        uint8_t talkCPU;
        uint8_t talkDEV;
        uint8_t connected;

        bool getTalkDev(uint8_t port);
        void setTalkCPU(uint8_t port, bool talk);
        void runStep();

    public:
        IOController();
        ~IOController();

        void reset();
        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);
        void setPwr(bool pwr);
        void refreshCycle(uint64_t cycle);

        bool getTalk(uint8_t port);
        void setTalk(uint8_t port, bool talk);
        void send(uint8_t port, uint8_t data);
       // uint8_t receive(uint8_t port);

        void addIO(std::shared_ptr<IODevice> device, uint8_t port);
        void removeIO(uint8_t port);

        uint8_t getOutBuf(uint8_t port, uint8_t index);
        uint8_t getInBuf(uint8_t port, uint8_t index);
    };
} // namespace computer

#endif // IOCONTROLLER_HPP