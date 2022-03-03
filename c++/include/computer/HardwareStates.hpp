#ifndef HARDWARESTATES_HPP
#define HARDWARESTATES_HPP

#include <memory>

#include <SFML/Audio.hpp>

#include "computer/Device.hpp"
#include "computer/IOController.hpp"
#include "computer/FDC.hpp"

namespace computer
{
    class HardwareStates : public computer::Device
    {
    private:
        uint8_t connected;
        uint8_t buzzerReg;
        uint8_t buzzerTime;
        uint64_t lastBuzzerCycle;
        std::shared_ptr<computer::IOController> ioCtrl;
        std::shared_ptr<computer::FDC> fdc;

        sf::SoundBuffer square;
        sf::Sound buzzer;
        sf::SoundSource::Status buzzerStatus;

    public:
        HardwareStates();
        ~HardwareStates();

        void setPwr(bool pwr);
        void reset();
        void run();
        void set(uint16_t adr, uint8_t data);
        uint8_t get(uint16_t adr);
        void refreshCycle(uint64_t cycle);

        void connect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
        void disconnect(std::shared_ptr<computer::Device> device, uint16_t startAdr, uint16_t endAdr);
    };
} // namespace computer

#endif // HARDWARESTATES_HPP