#ifndef FDC_HPP
#define FDC_HPP

#include "computer/Device.hpp"
#include "computer/FDD.hpp"
#include "computer/VRAM.hpp"
#include <memory>

namespace computer
{
    class FDC: public Device
    {
    private:
        std::shared_ptr<computer::FDD> fdd;
        std::shared_ptr<computer::VRAM> vram;
        uint8_t fddState;
        uint8_t fddCtrl;
        uint8_t sector;
        uint8_t track;
        uint8_t cmd;
        uint8_t cmdData;

        void MFMread();
        void MFMwrite();

    public:
        FDC(std::shared_ptr<computer::FDD> fdd, std::shared_ptr<computer::VRAM> ram);
        ~FDC();

        void setPwr(bool pwr);
        void refreshCycle(uint64_t cycle);

        void reset();
        void run();
        uint8_t get(uint16_t adr);
        void set(uint16_t adr, uint8_t data);

        bool isFDDLock();
        bool isFDDActive();
        std::shared_ptr<computer::FDD> getFDD();
    };
}

#endif // FDC_HPP