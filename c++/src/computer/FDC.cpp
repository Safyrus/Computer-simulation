#include "computer/FDC.hpp"
#include "utils/console.hpp"

#include <fstream>

computer::FDC::FDC(std::shared_ptr<computer::FDD> fdd, std::shared_ptr<computer::VRAM> ram)
{
    this->fdd = fdd;
    this->vram = ram;
    type = "FDC";
    name = "FDC";
    reset();
    set(5, 0);
}

computer::FDC::~FDC()
{
    fdd->setPwr(false);
    fdd->useLock(false);
    fdd->eject();
    fdd.reset();
}

void computer::FDC::MFMread()
{
    if (fdd)
    {
        //std::ofstream fileDebug;
        //fileDebug.open("debugRead.txt");

        vram->setLock(true);
        cmd = 0x80;
        uint8_t rawBuf = 0;
        uint8_t rawIndex = 0;
        uint8_t dataBuf = 0;
        uint16_t ramIndex = 0;
        uint8_t dataIndex = 0;
        bool wrongEncoding = false;
        for (uint16_t i = 0; i < 12500; i++)
        {
            bool data = fdd->FDCget(track, sector, i);
            rawBuf = (rawBuf << 1) + data;
            rawIndex++;
            if (rawIndex >= 2)
            {
                switch (rawBuf % 8)
                {
                case 0b010:
                case 0b100:
                    data = 0;
                    break;
                case 0b001:
                case 0b101:
                    data = 1;
                    break;
                default:
                    if(!wrongEncoding)
                    {
                        printError("FDC: wrong encoding !");
                        wrongEncoding = true;
                        cmd |= 0x40;
                    }
                    data = rand() % 2;
                    break;
                }
                //fileDebug << "#" << ((data) ? "1" : "0");
                rawIndex = 0;
                dataBuf = (dataBuf << 1) + data;
                dataIndex++;
            }

            if (dataIndex >= 8)
            {
                //fileDebug << (int)dataBuf << "\n!";
                vram->unlockSet(ramIndex, dataBuf);
                dataBuf = 0;
                dataIndex = 0;
                ramIndex++;
                if (ramIndex >= vram->length())
                {
                    break;
                }
            }
        }
        //fileDebug.close();
    }
}

#include <fstream>

void computer::FDC::MFMwrite()
{
    if (fdd)
    {
        vram->setLock(true);
        cmd = 0x80;

        //std::ofstream fileDebug;
        //fileDebug.open("debugWrite.txt");

        bool bit = 0;
        uint8_t rawBuf = 0;
        uint8_t rawIndex = 0;
        uint8_t dataBuf = 0;
        uint16_t ramIndex = 0;
        uint8_t dataIndex = 0;
        bool data = 0;
        fdd->FDCset(track, sector, 0, bit);

        for (uint16_t i = 0; i < 12500; i++)
        {
            if (dataIndex == 0 && rawIndex == 0)
            {
                if (ramIndex >= vram->length())
                {
                    dataBuf = 0;
                }
                else
                {
                    dataBuf = vram->unlockGet(ramIndex);
                }
                dataIndex = 8;
                ramIndex++;
                //fileDebug << "\n!" << (int)dataBuf;
            }

            if (rawIndex == 0)
            {
                data = ((dataBuf >> (dataIndex - 1)) & 0x01);
                //fileDebug << "#" << ((data) ? "1" : "0") << "|" << (int)((dataBuf >> (dataIndex - 1)) & 0x01) << "#";
                dataIndex--;
            }

            if (data)
            {
                if (rawIndex == 1)
                {
                    rawBuf = (rawBuf << 1) + 1;
                    fdd->FDCset(track, sector, i, bit);
                    bit = !bit;
                    //fileDebug << ((bit) ? "1" : "0") << "b";
                    rawIndex = 0;
                }
                else
                {
                    rawBuf = (rawBuf << 1) + 0;
                    fdd->FDCset(track, sector, i, bit);
                    //fileDebug << ((bit) ? "1" : "0") << "a";
                    rawIndex = 1;
                }
            }
            else
            {
                bool one = rawBuf & ((0x01) << rawIndex);
                if (one)
                {
                    if (rawIndex == 0)
                    {
                        rawBuf = (rawBuf << 1) + 0;
                        fdd->FDCset(track, sector, i, bit);
                        //fileDebug << ((bit) ? "1" : "0") << "c";
                        rawIndex = 1;
                    }
                    else if (rawIndex == 1)
                    {
                        rawBuf = (rawBuf << 1) + 0;
                        fdd->FDCset(track, sector, i, bit);
                        //fileDebug << ((bit) ? "1" : "0") << "d";
                        rawIndex = 0;
                    }
                }
                else
                {
                    if (rawIndex == 0)
                    {
                        rawBuf = (rawBuf << 1) + 1;
                        fdd->FDCset(track, sector, i, bit);
                        bit = !bit;
                        //fileDebug << ((bit) ? "1" : "0") << "e";
                        rawIndex = 1;
                    }
                    else if (rawIndex == 1)
                    {
                        rawBuf = (rawBuf << 1) + 0;
                        fdd->FDCset(track, sector, i, bit);
                        //fileDebug << ((bit) ? "1" : "0") << "f";
                        rawIndex = 0;
                    }
                }
            }
        }
        // fileDebug.close();
    }
}

void computer::FDC::reset()
{
    fddState = 0;
    fddCtrl = 0;
    sector = 0;
    track = 0;
    if (fdd)
    {
        fdd->reset();
    }
}

void computer::FDC::run()
{
}

void computer::FDC::setPwr(bool pwr)
{
    this->pwr = pwr;
    if (fdd)
    {
        fdd->setPwr(pwr);
    }
}

void computer::FDC::refreshCycle(uint64_t cycle)
{
    cycleCPU = cycle;
    if (fdd)
    {
        fdd->refreshCycle(cycle);
        if ((cmd & 0x80) && !fdd->isFDCLock())
        {
            cmd &= 0b01111111;
            vram->setLock(false);
        }
    }
}

uint8_t computer::FDC::get(uint16_t adr)
{
    uint8_t ret = 0;
    switch (adr)
    {
    case 0:
        ret = cmd;
        break;
    case 1:
        ret = cmdData;
        break;
    case 2:
        ret = track;
        break;
    case 3:
        ret = sector;
        break;
    case 4:
        fddState = 0;
        if (fdd)
        {
            for (uint8_t i = 0; i < 8; i++)
            {
                fddState |= (fdd->get(i) << i);
            }
        }
        ret = fddState;
        break;
    case 5:
        fddCtrl = (fddCtrl & 0b11011111) | (fdd->isHeadMoving() << 5);
        ret = fddCtrl;
        break;
    default:
        printDebug("FDC: Nothing at adr " + std::to_string(adr));
        break;
    }
    return ret;
}

void computer::FDC::set(uint16_t adr, uint8_t data)
{
    uint8_t newData;
    switch (adr)
    {
    case 0:
        cmd = data;
        switch (cmd)
        {
        case 4:
            MFMread();
            break;
        case 8:
            MFMwrite();
            break;
        }
        break;
    case 1:
        cmdData = data;
        break;
    case 2:
        track = data;
        break;
    case 3:
        sector = data;
        break;
    case 4:
        printDebug("FDC: Can't set read only FDD State register");
        break;
    case 5:
        newData = fddCtrl ^ data;
        if (fdd)
        {
            for (uint8_t i = 0; i < 8; i++)
            {
                if ((newData >> i) & 0x01)
                {
                    bool d = (data >> i) & 0x01;
                    fdd->set(i, d);
                }
            }
        }
        fddCtrl = data;
        break;
    default:
        printDebug("FDC: Nothing at adr " + std::to_string(adr));
        break;
    }
}

bool computer::FDC::isFDDLock()
{
    if (fdd)
    {
        return fdd->isLock();
    }
    return false;
}

bool computer::FDC::isFDDActive()
{
    if (fdd)
    {
        return fdd->isActiveLED();
    }
    return false;
}
