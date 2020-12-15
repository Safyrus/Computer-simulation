#ifndef CPU_HPP
#define CPU_HPP

#include <vector>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

namespace computer
{
    class CPU
    {
    private:
        uint8_t bus[1024 * 64];

    public:
        unsigned int cycle;
        uint16_t pc;
        uint8_t reg[16];

        CPU();
        ~CPU();

        void reset();
        uint8_t getBusData(uint16_t adr);
        void setBusData(uint16_t adr, uint8_t data);
        void loadOnBus(uint16_t start, std::vector<uint8_t> data);
    };
} // namespace computer

#endif // CPU_HPP