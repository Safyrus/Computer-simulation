#ifndef RAM_HPP
#define RAM_HPP

#include <vector>
#include <string>

#include "computer/Device.hpp"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace computer
{
    class RAM : public computer::Device
    {
    private:
        uint8_t *data;
        uint16_t len;
        uint32_t hz;
        std::vector<uint8_t> dataBuf;
        std::vector<uint16_t> adrBuf;

    public:
        RAM();
        RAM(uint16_t size);
        RAM(uint16_t size, uint32_t hz);
        ~RAM();

        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);
        void load(std::string filePath);
    };
} // namespace computer

#endif // RAM_HPP