#ifndef TESTDYNARECS_HPP
#define TESTDYNARECS_HPP

#include <string>

#include "computer/CPU.hpp"

void printCPU(std::shared_ptr<computer::CPU> cpu);

void printTranslaterResultCode(int res);

void testBuffer();

void testEmitter();

void testTranslater();

void testTranslater2();

void testDeviceThread(std::string filePath, uint32_t hz);

#endif // TESTDYNARECS_HPP