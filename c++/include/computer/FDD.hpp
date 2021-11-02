#ifndef FDD_HPP
#define FDD_HPP

#include "computer/Device.hpp"
#include "data/Floppy.hpp"
#include <memory>
#include <chrono>

namespace computer
{
    class FDD: public Device
    {
    private:
        const float RPM = 360;

        std::shared_ptr<data::Floppy> floppy;
        bool motor;
        bool stepMotor;
        bool stepMotorDirection;
        bool trackZeroLED;
        bool sectorZeroLED;
        bool writeProtectionLED;
        bool activeLED;
        bool lock;
        bool headWrite;
        bool lastData;
        uint32_t lastMotorIndex;
        uint32_t lastDataIndex;
        uint64_t lastDataCycle;
        uint64_t lastCPUCycle;
        uint64_t lastFDCCycle;
        uint64_t FDCCycleTime;

        uint64_t motorTimeCycle;
        uint64_t stepMotorTimeCycle;

        bool FDCLock;

        void setData(bool data);
        void setWriteProtectedLED(bool on);
        void setTrackZeroLED(bool on);
        void setSectorZeroLED(bool on);
        void setMotor(bool on);
        void setStep(bool on);
        void setDirection(bool up);
        void setReadWrite(bool write);

        bool getData();
        bool isWriteProtected();
        bool isTrackZero();
        bool isSectorZero();

        void doLastWrite(bool check = true);

    public:
        FDD();
        ~FDD();

        void setPwr(bool pwr);
        void reset();
        void run();
        uint8_t get(uint16_t adr);
        void set(uint16_t adr, uint8_t data);
        bool FDCget(uint8_t track, uint8_t sector, uint16_t offset);
        void FDCset(uint8_t track, uint8_t sector, uint16_t offset, uint8_t data);

        void insert(std::shared_ptr<data::Floppy> floppy);
        void eject();
        void useLock(bool lock);
        bool isLock();

        bool isActiveLED();
        bool isHeadMoving();

        bool isFDCLock();
        void refreshCycle(uint64_t cycle);
    };
}

#endif // FDD_HPP