#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "computer/Computer.hpp"
#include "global.hpp"

Computer::Computer()
{
    Hz = 10;
    cycleCount = 0;
    cpu = new CPU();
    pause = false;
}

Computer::Computer(int hz)
{
    Hz = hz;
    cycleCount = 0;
    cpu = new CPU();
    pause = false;
}

Computer::~Computer()
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        delete devices[i];
    }
    delete cpu;
}

void Computer::halfCycle()
{
    if (!pause)
    {

        bool clkBefore = cpu->getClk();
        while (cpu->getClk() == clkBefore)
        {
            cycle();
        }
        if (print_debug)
            std::cout << cpu->getClk() << "  " << cpu->getStep() << "\n"
                      << std::flush;
        if (!cpu->getClk())
        {
            cycleCount++;
        }
    }
}

void Computer::cycle()
{
    if (!pause)
    {
        int dev = -1;
        for (unsigned int i = 0; i < devices.size(); i++)
        {
            if (cpu->getAdr() >= adrDeviceStart[i] && cpu->getAdr() <= adrDeviceEnd[i])
            {
                dev = i;
                break;
            }
        }

        if (dev != -1)
        {
            devices[dev]->setAdr(cpu->getAdr() - adrDeviceStart[dev]);
            int data4 = devices[dev]->getData4();
            if (print_debug)
                std::cout << "data: " << data4 << std::endl;
            if (!cpu->getClk())
            {
                if (print_debug)
                    std::cout << "Clock off" << std::endl;
                cpu->setData(data4);
                cpu->setClk();
                cpu->stp();
            }
            else
            {
                if (print_debug)
                    std::cout << "Clock on" << std::endl;
                switch (cpu->getStep())
                {
                case 0:
                    if (print_debug)
                        std::cout << "step 0" << std::endl;
                    cpu->setClk();
                    cpu->stp();
                    break;
                case 1:
                    if (print_debug)
                        std::cout << "step 1" << std::endl;
                    if (cpu->getLoad())
                    {
                        if (print_debug)
                            std::cout << "DATA load: " << devices[dev]->getData() << std::endl;
                        cpu->setData(devices[dev]->getData());
                    }
                    else
                    {
                        int data = cpu->getData();
                        if (print_debug)
                            std::cout << "DATA save: " << data << std::endl;
                        devices[dev]->setData((data & 0xff));
                    }
                    cpu->stp();
                    break;
                case 2:
                    if (print_debug)
                        std::cout << "step 2" << std::endl;
                    cpu->setClk();
                    cpu->stp();
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            cpu->setClk();
            cpu->stp();
        }

        if (print_debug)
            std::cout << "CPU: adr[" << cpu->getAdr() << "]  data[" << cpu->getData() << "] step[" << cpu->getStep() << "]  clk[" << cpu->getClk() << "]  load[" << cpu->getLoad() << "]  pwr[" << cpu->getPwr() << "]" << std::endl;
    }
}

void Computer::setPwr()
{
    cpu->setPwr();
    if(cpu->getPwr())
    {
        cycleCount = 0;
        for (unsigned int i = 0; i < devices.size(); i++)
        {
            devices[i]->reset();
        }
    }
}

bool Computer::getPwr()
{
    return cpu->getPwr();
}

void Computer::setPause()
{
    pause = !pause;
}

bool Computer::getPause()
{
    return pause;
}

void Computer::addDevice(Device *d, int adrStart, int adrEnd)
{
    devices.push_back(d);
    adrDeviceStart.push_back(adrStart);
    adrDeviceEnd.push_back(adrEnd);
}

void Computer::removeDevice(Device *d)
{
    std::vector<Device *>::iterator itDev;
    std::vector<int>::iterator itStart;
    std::vector<int>::iterator itEnd;
    itStart = adrDeviceStart.begin();
    itEnd = adrDeviceEnd.begin();

    for (itDev = devices.begin(); itDev != devices.end(); itDev++)
    {
        if (d == *itDev)
        {
            devices.erase(itDev);
            adrDeviceStart.erase(itStart);
            adrDeviceEnd.erase(itEnd);
            break;
        }
        itStart++;
        itEnd++;
    }
}

void Computer::setHz(unsigned int hz)
{
    Hz = hz;
}

void Computer::setCycle(unsigned int c)
{
    cycleCount = c;
}

unsigned int Computer::getHz()
{
    return Hz;
}

unsigned int Computer::getCycle()
{
    return cycleCount;
}

void Computer::print(int x, int y)
{
    std::stringstream ss;
    ss << "\x1b[" << y << ";" << x << "H";
    std::cout << ss.str() << "|COM:  hz=" << Hz << "  cycle=" << cycleCount;
    ss << "\x1b[" << y + 1 << ";" << x << "H";
    std::cout << ss.str() << "|CPU:  stp=" << cpu->getStep() << "  clk=" << cpu->getClk() << "  pwr=" << cpu->getPwr() << "  load=" << cpu->getLoad();
    ss.clear();
    ss << "\x1b[" << y + 2 << ";" << x << "H";
    std::cout << ss.str() << "|ADR:  " << std::setfill('0') << std::setw(4) << cpu->getAdr();
    ss.clear();
    ss << "\x1b[" << y + 3 << ";" << x << "H";
    std::cout << ss.str() << "|DATA: ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData() >> 24) & 0xff) << "  ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData() >> 16) & 0xff) << "  ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData() >> 8) & 0xff) << "  ";
    std::cout << std::setfill('0') << std::setw(2) << ((cpu->getData()) & 0xff);
    std::cout << std::flush;
    cpu->print(x, y + 4);
}

void Computer::display(sf::RenderWindow &window, int x, int y)
{
    int charSizeX = 8;
    int charSizeY = 10;
    sf::Text text;
    text.setFont(baseFont);
    text.setCharacterSize(20);
    text.setScale(0.5, 0.5);

    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);

    std::stringstream ss;
    ss << "   HZ=" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << Hz;
    ss << "  CLK #";
    ss << "\nCYCLE=" << std::uppercase << std::setfill('0') << std::setw(8) << cycleCount;
    ss << "  PWR #";
    ss << "\n  STP=" << std::uppercase << std::setfill('0') << std::setw(2) << cpu->getStep();
    ss << "        LD  #";
    ss << "\n  ADR=" << std::uppercase << std::setfill('0') << std::setw(4) << cpu->getAdr();
    ss << "      PS  #";
    ss << "\n DATA=";
    ss << std::uppercase << std::setfill('0') << std::setw(2) << ((cpu->getData() >> 24) & 0xff) << " ";
    ss << std::uppercase << std::setfill('0') << std::setw(2) << ((cpu->getData() >> 16) & 0xff) << " ";
    ss << std::uppercase << std::setfill('0') << std::setw(2) << ((cpu->getData() >> 8) & 0xff) << " ";
    ss << std::uppercase << std::setfill('0') << std::setw(2) << ((cpu->getData()) & 0xff);

    text.setString(ss.str());
    window.draw(text);

    sf::RectangleShape rect(sf::Vector2f(charSizeX, charSizeY));
    if (cpu->getClk())
    {
        rect.setFillColor(sf::Color::Green);
    }
    else
    {
        rect.setFillColor(sf::Color::Red);
    }
    rect.setPosition(x + charSizeX * 20, y + charSizeY * 0);
    window.draw(rect);

    if (cpu->getPwr())
    {
        rect.setFillColor(sf::Color::Green);
    }
    else
    {
        rect.setFillColor(sf::Color::Red);
    }
    rect.setPosition(x + charSizeX * 20, y + charSizeY * 1);
    window.draw(rect);

    if (cpu->getLoad())
    {
        rect.setFillColor(sf::Color::Green);
    }
    else
    {
        rect.setFillColor(sf::Color::Red);
    }
    rect.setPosition(x + charSizeX * 20, y + charSizeY * 2);
    window.draw(rect);

    if (pause)
    {
        rect.setFillColor(sf::Color::Green);
    }
    else
    {
        rect.setFillColor(sf::Color::Red);
    }
    rect.setPosition(x + charSizeX * 20, y + charSizeY * 3);
    window.draw(rect);

    cpu->display(window, x + charSizeX * 0, y + charSizeY * 5);
}