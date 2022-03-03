#include "computer/HardwareStates.hpp"
#include <math.h>
#include "utils/console.hpp"

computer::HardwareStates::HardwareStates()
{
    type = "HWSTATS";
    name = "HWSTATS";
    std::string snd = "data/snd/square.wav";
    if (!square.loadFromFile(snd))
    {
        printError("HWSTATES: Cannot load sound file '" + snd + "'");
    }
    reset();
    buzzer.setBuffer(square);
    buzzer.setLoop(true);
}

computer::HardwareStates::~HardwareStates()
{
}

void computer::HardwareStates::setPwr(bool pwr)
{
    this->pwr = pwr;
    buzzer.stop();
}

void computer::HardwareStates::reset()
{
    buzzerReg = 0;
    buzzerTime = 0;
    lastBuzzerCycle = 0;
}

void computer::HardwareStates::run()
{
}

void computer::HardwareStates::set(uint16_t adr, uint8_t data)
{
    if(!pwr)
        return;
    uint16_t adrTo = adr / 8;
    uint8_t reg = adr % 8;
    switch (adrTo)
    {
    case 0:
        if (reg == 2)
        {
            buzzerReg = data;
            lastBuzzerCycle = cycleCPU;
            buzzerTime = (buzzerReg >> 4);
            //float sec = 2 - ((buzzerReg >> 4) * (1.0 / 8));
            float pitch = (buzzerReg & 0x07);
            if (pitch == 0)
            {
                pitch = 1;
            }
            else if (buzzerReg & 0x08)
            {
                pitch = 1.0 / (pitch+1);
            }
            else
            {
                pitch = 1.0 * (pitch+1);
            }
            buzzerStatus = buzzer.getStatus();
            if(buzzerStatus != sf::Sound::Playing)
            {
                printDebug("HWSTATES(SND): START");
                buzzer.play();
                buzzerStatus = buzzer.getStatus();
            }
            //buzzer.setPlayingOffset(sf::seconds(sec));
            buzzer.setPitch(pitch);
        }
        break;
    case 1:
        if (fdc)
        {
            fdc->set(reg, data);
        }
        break;
    case 2:
        if (ioCtrl)
        {
            ioCtrl->set(reg + 32, data);
        }
        break;
    default:
        break;
    }
}

uint8_t computer::HardwareStates::get(uint16_t adr)
{
    if(!pwr)
        return 0;
    uint16_t adrTo = adr / 8;
    uint8_t reg = adr % 8;
    uint8_t ret = 0;
    switch (adrTo)
    {
    case 0:
        switch (adr % 8)
        {
        case 0:
            ret = connected;
            break;
        case 1:
            ret = cycleCPU / (1000000 / 256);
            break;
        case 2:

            ret = buzzerReg;
            break;
        default:
            break;
        }
        break;
    case 1:
        if (fdc)
        {
            ret = fdc->get(reg);
        }
        break;
    case 2:
        if (ioCtrl)
        {
            ret = ioCtrl->get(reg + 32);
        }
        break;
    default:
        break;
    }
    return ret;
}

void computer::HardwareStates::connect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr)
{
    std::string type = device->getType();
    if (startAdr >= 0x8000)
    {
        connected |= 0x80;
    }
    else if (type.compare("ROM") == 0)
    {
        if (endAdr <= 0xFFF)
        {
            connected |= 0x01;
        }
        else if (startAdr >= 0x1000 && endAdr <= 0x17FF)
        {
            connected |= 0x02;
        }
    }
    else if (type.compare("RAM") == 0)
    {
        if (startAdr >= 0x1800 && endAdr <= 0x1BFF)
        {
            connected |= 0x04;
        }
        else if (startAdr >= 0x4000 && endAdr <= 0x7FFF)
        {
            connected |= 0x08;
        }
    }
    else if (type.compare("VRAM") == 0)
    {
        if (startAdr >= 0x2000 && endAdr <= 0x3FFF)
        {
            connected |= 0x10;
        }
    }
    else if (type.compare("IOCTRL") == 0)
    {
        connected |= 0x40;
        ioCtrl = std::dynamic_pointer_cast<computer::IOController>(device);
    }
    else if (type.compare("FDC") == 0)
    {
        connected |= 0x20;
        fdc = std::dynamic_pointer_cast<computer::FDC>(device);
    }
}

void computer::HardwareStates::disconnect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr)
{
    std::string type = device->getType();
    if (type.compare("ROM") == 0)
    {
        if (endAdr <= 0xFFF)
        {
            connected &= 0xFE;
        }
        else if (startAdr >= 0x1000 && endAdr <= 0x17FF)
        {
            connected &= 0xFD;
        }
    }
    else if (type.compare("RAM") == 0)
    {
        if (startAdr >= 0x1800 && endAdr <= 0x1BFF)
        {
            connected &= 0xFB;
        }
        else if (startAdr >= 0x4000 && endAdr <= 0x77FF)
        {
            connected &= 0xF7;
        }
    }
    else if (type.compare("VRAM") == 0)
    {
        if (startAdr >= 0x2000 && endAdr <= 0x3FFF)
        {
            connected &= 0xEF;
        }
    }
    else if (type.compare("IOCTRL") == 0)
    {
        ioCtrl = nullptr;
        connected &= 0xBF;
    }
    else if (type.compare("FDC") == 0)
    {
        connected &= 0xDF;
        fdc = nullptr;
    }
    else if (startAdr >= 0x8000)
    {
        connected &= 0x7F;
    }
}

void computer::HardwareStates::refreshCycle(uint64_t cycle)
{
    cycleCPU = cycle;
    uint64_t buzzerRemaining = 0;
    if ((lastBuzzerCycle + (buzzerTime * 125000)) > cycleCPU)
    {
        buzzerRemaining = ((lastBuzzerCycle + (buzzerTime * 125000)) - cycleCPU);
    }
    int tmp = 0;
    if (buzzerRemaining != 0)
    {
        tmp = (buzzerRemaining/125000)+1;
    }
    buzzerReg = (buzzerReg & 0x0F) | (tmp << 4);
    if((buzzerReg & 0xF0) == 0 && buzzerStatus != sf::Sound::Stopped && (buzzerStatus = buzzer.getStatus()) == sf::Sound::Playing)
    {
        printDebug("HWSTATES(SND): STOP");
        buzzer.stop();
        buzzerStatus = buzzer.getStatus();
    }
}
