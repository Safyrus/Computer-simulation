#pragma once
#include <string>

class DISK
{
private:
    int8_t *tab;
    int adr;
    int len;

public:
    DISK(int l);
    ~DISK();

    int getAdr();
    void setAdr(int a);
    int8_t getData();
    int getData4();
    void setData(int8_t d);
    int getLen();

    bool load(const char *f);
    bool save(const char *f);
};