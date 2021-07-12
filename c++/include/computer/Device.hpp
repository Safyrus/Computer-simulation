#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <string>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

namespace computer
{
    class Device
    {
    protected:
        bool pwr;
        std::string type;
        std::string name;
        uint64_t cycleCPU;

    public:
        bool running;

        Device();
        virtual ~Device();

        virtual void setPwr(bool pwr);
        virtual bool getPwr();
        virtual void setName(std::string name);
        virtual std::string getType();
        virtual std::string getName();

        virtual void reset() = 0;
        virtual void run() = 0;
        virtual uint8_t get(uint16_t adr) = 0;
        virtual void set(uint16_t adr, uint8_t data) = 0;
        virtual void refreshCycle(uint64_t cycle);
    };
} // namespace computer

#endif // DEVICE_HPP