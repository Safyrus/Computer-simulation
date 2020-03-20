#pragma once
#include <string>

class Device
{
protected:
    int8_t *data;
    int len;
    int adr;

public:
    Device();
    Device(int l);
    virtual ~Device();

    virtual int getAdr();
    virtual void setAdr(int a);
    virtual int8_t getData();
    virtual int getData4();
    virtual void setData(int8_t d);
    virtual int getLen();
};