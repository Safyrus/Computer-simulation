#ifndef RUNNABLEDEVICE_HPP
#define RUNNABLEDEVICE_HPP

#include <vector>
#include <memory>

#include "computer/Device.hpp"

#ifndef _WIN32
#include <thread>
#else
#include "mingw.thread.h"
#endif

namespace computer
{
    class RunnableDevice
    {
    protected:
        std::shared_ptr<computer::Device> device;

        std::thread thread;

    public:
        RunnableDevice(std::shared_ptr<computer::Device> device);
        ~RunnableDevice();

        void run();
        void join();
        void stop();
    };
} // namespace computer

#endif // RUNNABLEDEVICE_HPP