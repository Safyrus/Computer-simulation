#ifndef RUNS257INTERPRETER_HPP
#define RUNS257INTERPRETER_HPP

#include <string>

#include "computer_old/Computer.hpp"

#ifndef _WIN32
void run(Computer *com);
#else
#include <windows.h>

BOOLEAN nanosleepWin(LONGLONG ns);

DWORD WINAPI run(LPVOID lpParameter);
#endif

void runS257Interpreter(std::string progPath, int hz, int fps = 60);

#endif // RUNS257INTERPRETER_HPP