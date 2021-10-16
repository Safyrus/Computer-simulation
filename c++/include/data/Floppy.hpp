#ifndef FLOPPY_HPP
#define FLOPPY_HPP

#include <string>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

namespace data
{
    class Floppy
    {
    private:
        uint8_t *data;
        uint8_t trackCount;
        uint32_t bpt;
        uint8_t head;
        uint32_t index;
        bool writable;

    public:
        Floppy(uint8_t tracks = 40, uint32_t bpt = 100000, bool writable = true);
        ~Floppy();

        void write(bool data);
        bool read();
        void moveHead(bool forward);
        void setPos(uint8_t head, uint32_t index);
        void next();

        uint8_t getHead();
        uint32_t getIndex();
        bool isWritable();

        uint8_t getTrackCount();
        uint32_t getBpt();
        void load(std::string file);
        void save(std::string file);
    };
}

#endif // FLOPPY_HPP