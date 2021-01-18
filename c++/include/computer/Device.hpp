#ifndef DEVICE_HPP
#define DEVICE_HPP

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

enum DEVICE_TYPE{
    NOVAL = 0,
    CPU = 1,
    BUS = 2,
    RAM = 3,
    ROM = 4
};

namespace computer
{
    class Device
    {
    protected:
        DEVICE_TYPE type;
        bool pwr;

    public:
        bool running;

        Device();
        virtual ~Device();

        DEVICE_TYPE getType();
        void setPwr(bool pwr);
        bool getPwr();

        virtual void reset() = 0;
        virtual void run() = 0;
        virtual uint8_t get(uint16_t adr) = 0;
        virtual void set(uint16_t adr, uint8_t data) = 0;
    };
} // namespace computer

#endif // DEVICE_HPP