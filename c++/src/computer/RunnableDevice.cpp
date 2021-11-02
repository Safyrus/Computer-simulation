#include "computer/RunnableDevice.hpp"

#include <iostream>

computer::RunnableDevice::RunnableDevice(std::shared_ptr<computer::Device> device)
{
    this->device = device;
    this->device->running = false;
    hasStop = false;
}

computer::RunnableDevice::~RunnableDevice()
{
}

void computer::RunnableDevice::run()
{
    this->device->running = true;
    thread = std::thread(&Device::run, device);
    hasStop = false;
}

void computer::RunnableDevice::join()
{
    thread.join();
    hasStop = true;
}

void computer::RunnableDevice::stop()
{
    this->device->running = false;
}

bool computer::RunnableDevice::isStop()
{
    return hasStop;
}
