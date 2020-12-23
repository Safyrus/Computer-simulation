#ifndef CPU_HPP
#define CPU_HPP

#include <vector>

#include "computer/Bus.hpp"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace computer
{
    class CPU : public computer::Device, public std::enable_shared_from_this<CPU>
    {
    private:
        uint8_t rawBus[1024 * 64];
        std::shared_ptr<computer::Bus> bus;

    public:
        uint32_t cycle;
        uint16_t pc;
        uint8_t reg[16];

        CPU(std::shared_ptr<computer::Bus> bus);
        ~CPU();

        void run();
        uint8_t get(uint16_t adr);
        void set(uint16_t adr, uint8_t data);

        void reset();
        uint8_t getBusData(uint16_t adr);
        void setBusData(uint16_t adr, uint8_t data);
        void loadOnBus(uint16_t start, std::vector<uint8_t> data);
    };
} // namespace computer

#endif // CPU_HPP