#ifndef DEVICE_HPP
#define DEVICE_HPP

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

namespace computer
{
    class Device
    {
    public:
        bool running;

        Device();
        virtual ~Device();

        virtual void run() = 0;
        virtual uint8_t get(uint16_t adr) = 0;
        virtual void set(uint16_t adr, uint8_t data) = 0;
    };
} // namespace computer

#endif // DEVICE_HPP