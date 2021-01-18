#ifndef ROM_HPP
#define ROM_HPP

#include <vector>
#include <string>

#include "computer/Device.hpp"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace computer
{
    class ROM : public computer::Device
    {
    private:
        uint8_t *data;
        uint16_t len;
        uint32_t hz;

    public:
        ROM();
        ROM(uint16_t size);
        ROM(uint16_t size, uint32_t hz);
        ~ROM();

        void reset();

        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);
        void load(std::string filePath);
    };
} // namespace computer

#endif // ROM_HPP