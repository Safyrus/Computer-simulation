#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace dynarec
{
    class Buffer
    {
    protected:
        std::vector<uint8_t> mem;
        uint16_t startAdr;
        uint16_t insCount;

    public:
        Buffer(uint16_t startAdr, uint16_t insCount);
        ~Buffer();

        int execute();
        void write8(uint8_t data);
        void write32(uint32_t data);

        uint8_t *getPtr(); // TODO find a more secure way
        uint32_t getSize();
        uint16_t getStartAdr();
        uint16_t getInsCount();
        void setInsCount(uint16_t n);
        void incInsCount();
    };
} // namespace dynarec

#endif // BUFFER_HPP
