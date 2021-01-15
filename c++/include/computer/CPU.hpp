#ifndef CPU_HPP
#define CPU_HPP

#include <vector>

#include "computer/Bus.hpp"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#ifndef _WIN32
typedef unsigned long uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

namespace computer
{
    class CPU : public computer::Device, public std::enable_shared_from_this<CPU>
    {
    private:
        uint8_t rawBus[1024 * 64];
        std::shared_ptr<computer::Bus> bus;
        bool threadWanted;

    public:
        uint16_t pc;
        uint8_t reg[16];
        uint32_t hz;
        uint64_t cycle;

        CPU(std::shared_ptr<computer::Bus> bus);
        CPU(std::shared_ptr<computer::Bus> bus, bool threadWanted);
        ~CPU();

        void reset();
        void resetReg();
        void setPwr(bool pwr);

        void run();
        uint8_t get(uint16_t adr);
        void set(uint16_t adr, uint8_t data);

        uint8_t getBusData(uint16_t adr);
        void setBusData(uint16_t adr, uint8_t data);
        void loadOnBus(uint16_t start, std::vector<uint8_t> data);
    };
} // namespace computer

#endif // CPU_HPP