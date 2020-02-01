#pragma once
#include <string>
#include "Device.hpp"

class DISK : public Device
{
public:
    DISK(int l);
    ~DISK();

    bool load(const char *f);
    bool save(const char *f);
};