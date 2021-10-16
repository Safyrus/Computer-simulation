#include "data/Floppy.hpp"
#include "math.h"
#include "utils/console.hpp"
#include <fstream>

data::Floppy::Floppy(uint8_t tracks, uint32_t bpt, bool writable)
{
    printDebug("Floppy: Creation");
    uint32_t size = tracks * (uint32_t)ceil(bpt / 8);
    this->data = new uint8_t[size];
    for (uint32_t i = 0; i < size; i++)
    {
        data[i] = 0;
    }

    this->trackCount = tracks;
    this->bpt = bpt;
    this->head = 0;
    this->index = 0;
    this->writable = writable;
}

data::Floppy::~Floppy()
{
    delete data;
    printDebug("Floppy: Destruction");
}

void data::Floppy::write(bool data)
{
    if (!writable)
        return;
    uint32_t i = (head * (uint32_t)ceil(bpt / 8)) + (index / 8);
    if (data)
    {
        this->data[i] |= (0x01 << (index % 8));
    }
    else
    {
        this->data[i] &= (0xFF ^ (0x01 << (index % 8)));
    }
    next();
}

bool data::Floppy::read()
{
    uint32_t i = (head * (uint32_t)ceil(bpt / 8)) + (index / 8);
    bool ret = (data[i] >> (index % 8)) & 0x01;
    next();
    return ret;
}

void data::Floppy::moveHead(bool forward)
{
    if (forward && head < trackCount)
    {
        head++;
    }
    else if (head > 0)
    {
        head--;
    }
}

void data::Floppy::setPos(uint8_t head, uint32_t index)
{
    if (head >= trackCount)
    {
        head = trackCount - 1;
    }
    this->head = head;
    this->index = index % bpt;
}

void data::Floppy::next()
{
    index = (index + 1) % bpt;
}

uint8_t data::Floppy::getHead()
{
    return head;
}

uint32_t data::Floppy::getIndex()
{
    return index;
}

bool data::Floppy::isWritable()
{
    return writable;
}

uint8_t data::Floppy::getTrackCount()
{
    return trackCount;
}

uint32_t data::Floppy::getBpt()
{
    return bpt;
}

void data::Floppy::load(std::string file)
{
    printDebug("Floppy: Load floppy " + file);
    std::ifstream f;
    f.open(file, std::ios::binary);
    if (!f.is_open())
    {
        printError("Floppy: Can't open file " + file);
    }
    f.read((char *)data, trackCount * (uint32_t)ceil(bpt / 8));
    printDebug("Floppy: Load done");
}

void data::Floppy::save(std::string file)
{
    printDebug("Floppy: Save floppy " + file);
    std::ofstream f;
    f.open(file, std::ios::binary);
    if (!f.is_open())
    {
        printError("Floppy: Can't open file " + file);
    }
    f.write((char *)data, trackCount * (uint32_t)ceil(bpt / 8));
    printDebug("Floppy: Save done");
}
