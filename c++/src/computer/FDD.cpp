#include "computer/FDD.hpp"
#include "utils/console.hpp"
#include "global.hpp"

computer::FDD::FDD()
{
    reset();
    type = "FDD";
    name = "FDD";
    lock = false;
    headPos = 0;
}

computer::FDD::~FDD()
{
}

void computer::FDD::setPwr(bool pwr)
{
    this->pwr = pwr;
    if (!pwr)
    {
        doLastWrite(!FDCLock);
        motor = false;
        stepMotor = false;
        stepMotorDirection = false;
        trackZeroLED = false;
        sectorZeroLED = false;
        writeProtectionLED = false;
        activeLED = false;
        headWrite = false;
    }
}

void computer::FDD::setData(bool data)
{
    if (print_debug)
    {
        printDebug("FDD: set data");
    }
    doLastWrite(!FDCLock);
    lastData = data;
}

void computer::FDD::setWriteProtectedLED(bool on)
{
    if (print_debug)
    {
        std::string str = "FDD: set write LED ";
        str += (on ? "ON" : "OFF");
        printDebug(str);
    }
    writeProtectionLED = on;
}

void computer::FDD::setTrackZeroLED(bool on)
{
    if (print_debug)
    {
        std::string str = "FDD: set track zero LED ";
        str += (on ? "ON" : "OFF");
        printDebug(str);
    }
    trackZeroLED = on;
}

void computer::FDD::setSectorZeroLED(bool on)
{
    if (print_debug)
    {
        std::string str = "FDD: set sector zero LED ";
        str += (on ? "ON" : "OFF");
        printDebug(str);
    }
    sectorZeroLED = on;
}

void computer::FDD::setMotor(bool on)
{
    if (print_debug)
    {
        std::string str = "FDD: set motor ";
        str += (on ? "ON" : "OFF");
        printDebug(str);
    }
    doLastWrite(!FDCLock);
    motor = on;
    if (motor)
    {
        motorTimeCycle = cycleCPU;
    }
    else
    {
        uint32_t index = (cycleCPU - motorTimeCycle) / 2;
        lastMotorIndex = (lastMotorIndex + index);
        if (floppy)
        {
            lastMotorIndex %= floppy->getBpt();
        }
    }
}

void computer::FDD::setStep(bool on)
{
    if (stepMotor)
    {
        uint64_t timeToStep = 3333;
        uint64_t cycleSinceStep = (cycleCPU - stepMotorTimeCycle);
        if (cycleSinceStep >= timeToStep)
        {
            stepMotor = false;
            int dir = (stepMotorDirection) ? 1 : -1;
            if (dir + headPos < 0)
            {
                dir = 0;
            }
            headPos += dir;
            if (headPos >= 40)
            {
                headPos = 39;
            }
            if (floppy)
            {
                floppy->setPos(headPos, floppy->getIndex());
            }
            if (print_debug)
            {
                std::string str = "Head is now on track " + std::to_string(headPos);
                printDebug(str);
            }
        }
    }
    if (!stepMotor && on)
    {
        if (print_debug)
        {
            std::string str = "Head is now moving";
            printDebug(str);
        }
        stepMotor = true;
        stepMotorTimeCycle = cycleCPU;
    }
}

void computer::FDD::setDirection(bool up)
{
    stepMotorDirection = up;
    if (print_debug)
    {
        std::string str = "Head direction is now ";
        str += (stepMotorDirection) ? "UP" : "DOWN";
        printDebug(str);
    }
}

void computer::FDD::setReadWrite(bool write)
{
    doLastWrite(!FDCLock);
    if (print_debug)
    {
        std::string str = "Head is now on ";
        str += (write) ? "WRITE" : "READ";
        printDebug(str);
    }
    headWrite = write;
}

bool computer::FDD::getData()
{
    if (!lock)
    {
        printError("FDD: Can't read, lock is not down");
        return false;
    }
    if (headWrite)
    {
        printError("FDD: Can't read, read head not active");
        return false;
    }
    if (motor)
    {
        if (print_debug)
        {
            printDebug("FDD: get Data");
        }
        bool data = false;
        setStep(false);
        if (stepMotor)
        {
            if (print_debug)
            {
                printDebug("FDD: head is moving, return garbage");
            }
            return rand() % 2;
        }
        uint64_t cycleSinceMotor = (cycleCPU - motorTimeCycle);
        uint32_t index = (lastMotorIndex + (cycleSinceMotor / 2) - 1);
        if (floppy)
        {
            index %= floppy->getBpt();
            floppy->setPos(headPos, index);
            bool tmp1 = floppy->read();
            bool tmp2 = floppy->read();
            data = tmp1 ^ tmp2;
        }
        if (print_debug)
        {
            std::string str = "FDD: read ";
            str += (data ? "1" : "0");
            str += " at head " + std::to_string(headPos) + ", index " + std::to_string(index);
            printDebug(str);
        }
        return data;
    }
    else
    {
        printError("FDD: Can't read, motor not on");
        return false;
    }
}

bool computer::FDD::isWriteProtected()
{
    return (writeProtectionLED && (!floppy || floppy->isWritable()));
}

bool computer::FDD::isTrackZero()
{
    return (trackZeroLED && (!floppy || floppy->getHead() == 0));
}

bool computer::FDD::isSectorZero()
{
    bool indexHole = false;
    if (floppy)
    {
        setStep(false);
        uint8_t head = floppy->getHead();
        uint64_t cycleSinceMotor = (cycleCPU - motorTimeCycle);
        uint32_t index = (lastMotorIndex + (cycleSinceMotor / 2)) % floppy->getBpt();
        floppy->setPos(head, index);
        indexHole = (floppy->getIndex() <= 5 || floppy->getIndex() >= (floppy->getBpt() - 6));
    }
    return (sectorZeroLED && (!floppy || indexHole));
}

void computer::FDD::doLastWrite(bool check)
{
    if (check && !lock)
    {
        printError("FDD: Can't write, lock is not down");
    }
    else if (check && !headWrite)
    {
        printError("FDD: Can't write, write head not active");
    }
    else if (!floppy)
    {
        printError("FDD: Can't write, No floppy");
    }
    else if (!check || (check && motor))
    {
        if (print_debug)
        {
            printDebug("FDD: do last write");
        }
        setStep(false);
        if (stepMotor)
        {
            if (print_debug)
            {
                std::string str = "FDD: can't write when head is moving";
                printDebug(str);
            }
            lastDataCycle = cycleCPU;
            return;
        }
        uint64_t cycleSinceData = (cycleCPU - lastDataCycle);
        uint64_t timesToWrite = (cycleSinceData / 2);
        uint64_t cycleSinceMotor = (cycleCPU - motorTimeCycle);
        uint32_t index = (lastMotorIndex + (cycleSinceMotor / 2) - timesToWrite) % floppy->getBpt();
        //printDebug(std::to_string(cycleCPU) + "  " + std::to_string(lastDataCycle));
        floppy->setPos(headPos, index);
        for (uint64_t i = 0; i < timesToWrite; i++)
        {
            if (print_debug)
            {
                std::string str = "FDD: write ";
                str += (lastData ? "1" : "0");
                str += " at head " + std::to_string(headPos) + ", index " + std::to_string(floppy->getIndex());
                printDebug(str);
            }
            floppy->write(lastData);
        }
    }
    else
    {
        printError("FDD: Can't write, motor not on");
    }
    lastDataCycle = cycleCPU;
}

void computer::FDD::reset()
{
    motor = false;
    stepMotor = false;
    stepMotorDirection = false;
    trackZeroLED = false;
    sectorZeroLED = false;
    writeProtectionLED = false;
    activeLED = false;
    headWrite = false;
    lastDataIndex = 0;
    lastData = false;
    motorTimeCycle = 0;
    lastDataCycle = 0;
    stepMotorTimeCycle = 0;
    lastMotorIndex = 0;
    FDCLock = false;
}

void computer::FDD::run()
{
}

uint8_t computer::FDD::get(uint16_t adr)
{
    if (!pwr)
    {
        if (print_debug)
        {
            printDebug("FDD: can't get, no power");
        }
        return 0;
    }

    uint8_t ret = 0;
    switch (adr)
    {
    case 0:
        ret = getData();
        break;
    case 1:
        ret = isWriteProtected();
        break;
    case 2:
        ret = isTrackZero();
        break;
    case 3:
        ret = isSectorZero();
        break;
    case 4:
        ret = isLock();
        break;
    default:
        if (print_debug)
        {
            printDebug("FDD: Nothing at adr " + std::to_string(adr));
        }
        break;
    }
    return ret;
}

void computer::FDD::set(uint16_t adr, uint8_t data)
{
    if (!pwr)
    {
        if (print_debug)
        {
            printDebug("FDD: can't set, no power");
        }
        return;
    }

    switch (adr)
    {
    case 0:
        setData(data);
        break;
    case 1:
        setWriteProtectedLED(data);
        break;
    case 2:
        setTrackZeroLED(data);
        break;
    case 3:
        setSectorZeroLED(data);
        break;
    case 4:
        setMotor(data);
        break;
    case 5:
        setStep(data);
        break;
    case 6:
        setDirection(data);
        break;
    case 7:
        setReadWrite(data);
        break;
    default:
        if (print_debug)
        {
            printDebug("FDD: Nothing at adr " + std::to_string(adr));
        }
        break;
    }
}

bool computer::FDD::FDCget(uint8_t track, uint8_t sector, uint16_t offset)
{
    if (!pwr)
        return 0;

    if (!lock)
    {
        printError("FDD(FDCget): Can't read, lock is not down");
        return false;
    }

    if (!FDCLock)
    {
        if (print_debug)
        {
            printDebug("FDD(FDCget): get Data");
        }
        if (!floppy)
        {
            printWarning("FDD(FDCget): No floppy");
        }
        FDCLock = true;
        lastFDCCycle = cycleCPU;

        uint64_t cycleSinceMotor = (cycleCPU - motorTimeCycle);
        uint32_t index = (lastMotorIndex + (cycleSinceMotor / 2) - 1);
        if(floppy)
        {
            index %= floppy->getBpt();
        }
        uint32_t wantedIndex = (sector * 12500) + offset;
        if (wantedIndex < index)
        {
            if(floppy)
            {
                index += index + floppy->getBpt() - wantedIndex;
            }else
            {
                index += index + 100000 - wantedIndex;
            }
        }
        else
        {
            index = wantedIndex - index;
        }
        FDCCycleTime = (3333 * std::abs(track - headPos)) + (index * 2);

        stepMotor = false;

        headWrite = false;
        if (print_debug)
        {
            std::string str = "Head is now on ";
            str += (headWrite) ? "WRITE" : "READ";
            printDebug(str);
        }

        motor = true;
        if (print_debug)
        {
            std::string str = "FDD: set motor ";
            str += (motor ? "ON" : "OFF");
            printDebug(str);
        }
        if (motor)
        {
            motorTimeCycle = cycleCPU;
        }
        else
        {
            uint32_t index = (cycleCPU - motorTimeCycle) / 2;
            lastMotorIndex = (lastMotorIndex + index);
            if(floppy)
            {
                lastMotorIndex %= floppy->getBpt();
            }
        }
    }

    bool data = false;
    if (floppy)
    {
        floppy->setPos(track, (sector * 12500) + offset);
        bool tmp1 = floppy->read();
        bool tmp2 = floppy->read();
        data = tmp1 ^ tmp2;
    }

    if (print_debug)
    {
        uint32_t index = 0;
        if (floppy)
        {
            index = floppy->getIndex();
        }
        std::string str = "FDD(FDCget): read ";
        str += (data ? "1" : "0");
        str += " at head " + std::to_string(track) + ", index " + std::to_string(index);
        //printDebug(str);
    }

    return data;
}

void computer::FDD::FDCset(uint8_t track, uint8_t sector, uint16_t offset, uint8_t data)
{
    if (!pwr)
        return;

    if (!lock)
    {
        printError("FDD(FDCset): Can't write, lock is not down");
        return;
    }

    if (!floppy)
    {
        printError("FDD(FDCset): Can't write, no floppy");
        return;
    }

    if (!FDCLock)
    {
        if (print_debug)
        {
            printDebug("FDD(FDCset): set Data");
        }
        FDCLock = true;
        lastFDCCycle = cycleCPU;
        uint32_t bpt = floppy->getBpt();

        uint8_t head = floppy->getHead();
        uint64_t cycleSinceMotor = (cycleCPU - motorTimeCycle);
        uint32_t index = (lastMotorIndex + (cycleSinceMotor / 2) - 1) % bpt;
        uint32_t wantedIndex = (sector * (bpt / 8)) + offset;
        if (wantedIndex < index)
        {
            index += index + bpt - wantedIndex;
        }
        else
        {
            index = wantedIndex - index;
        }
        FDCCycleTime = (3333 * std::abs(track - head)) + (index * 2);

        stepMotor = false;

        headWrite = true;
        if (print_debug)
        {
            std::string str = "Head is now on ";
            str += (headWrite) ? "WRITE" : "READ";
            printDebug(str);
        }

        motor = true;
        if (print_debug)
        {
            std::string str = "FDD: set motor ";
            str += (motor ? "ON" : "OFF");
            printDebug(str);
        }
        if (motor)
        {
            motorTimeCycle = cycleCPU;
        }
        else
        {
            uint32_t index = (cycleCPU - motorTimeCycle) / 2;
            lastMotorIndex = (lastMotorIndex + index) % bpt;
        }
    }

    floppy->setPos(track, (sector * 12500) + offset);
    floppy->write(data);

    if (print_debug)
    {
        std::string str = "FDD(FDCset): write ";
        str += (data ? "1" : "0");
        str += " at head " + std::to_string(track) + ", index " + std::to_string(floppy->getIndex());
        //printDebug(str);
    }
}

void computer::FDD::insert(std::shared_ptr<data::Floppy> floppy)
{
    if (print_debug)
    {
        printDebug("FDD: insert floppy");
    }
    if (lock)
    {
        printError("FDD: Can't insert floppy, lock is down");
        return;
    }
    if (this->floppy)
    {
        printError("FDD: Can't insert floppy, there is already one");
        return;
    }
    this->floppy = floppy;
}

void computer::FDD::eject()
{
    if (lock)
    {
        printError("FDD: Can't eject floppy, lock is down");
        return;
    }
    if (!floppy)
    {
        printError("FDD: Can't eject non existing floppy");
        return;
    }
    if (print_debug)
    {
        printDebug("FDD: eject floppy");
    }
    doLastWrite(false);
    this->floppy->save("floppy.img");
    this->floppy = nullptr;
}

void computer::FDD::useLock(bool lock)
{
    doLastWrite(!FDCLock);
    this->lock = lock;
    if (print_debug)
    {
        std::string str = "FDD: lock is now ";
        str += (this->lock) ? "DOWN" : "UP";
        printDebug(str);
    }
}

bool computer::FDD::isActiveLED()
{
    activeLED = motor | stepMotor;
    return activeLED;
}

bool computer::FDD::isHeadMoving()
{
    setStep(false);
    return stepMotor;
}

bool computer::FDD::isLock()
{
    return lock;
}

bool computer::FDD::isFloppyIn()
{
    return (floppy != nullptr);
}

void computer::FDD::refreshCycle(uint64_t cycle)
{
    lastCPUCycle = cycleCPU;
    cycleCPU = cycle;
    if (FDCLock && lastFDCCycle + FDCCycleTime < cycleCPU)
    {
        FDCLock = false;
        stepMotor = false;

        headWrite = false;
        if (print_debug)
        {
            std::string str = "Head is now on ";
            str += (headWrite) ? "WRITE" : "READ";
            printDebug(str);
        }

        motor = false;
        if (print_debug)
        {
            std::string str = "FDD: set motor ";
            str += (motor ? "ON" : "OFF");
            printDebug(str);
        }
        if (motor)
        {
            motorTimeCycle = cycleCPU;
        }
        else
        {
            uint32_t index = (cycleCPU - motorTimeCycle) / 2;
            lastMotorIndex = (lastMotorIndex + index);
            if (floppy)
            {
                lastMotorIndex %= floppy->getBpt();
            }
        }
        if (floppy)
        {
            floppy->save("floppy.img");
        }
    }
    if (lastCPUCycle > cycleCPU)
    {
        reset();
    }
}

bool computer::FDD::isFDCLock()
{
    return FDCLock;
}
