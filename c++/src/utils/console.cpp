#ifndef LINEARNODE_H
#define LINEARNODE_H

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <iomanip>

#include "utils/console.hpp"
#include "global.hpp"

#ifdef _WIN32
#include <windows.h>

// Some old MinGW/CYGWIN distributions don't define this:
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

static HANDLE stdoutHandle;
static DWORD outModeInit;

void setupConsole(void)
{
    DWORD outMode = 0;
    stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (stdoutHandle == INVALID_HANDLE_VALUE)
    {
        exit(GetLastError());
    }

    if (!GetConsoleMode(stdoutHandle, &outMode))
    {
        exit(GetLastError());
    }

    outModeInit = outMode;

    // Enable ANSI escape codes
    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(stdoutHandle, outMode))
    {
        exit(GetLastError());
    }
}

void restoreConsole(void)
{
    printf("\x1b[0m");

    // Reset console mode
    if (!SetConsoleMode(stdoutHandle, outModeInit))
    {
        exit(GetLastError());
    }
}

void SetWindow(int Width, int Height)
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE); // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);       // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);       // Set Window Size
}

void rawConsole(bool raw)
{
}

#else
#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>

static struct termios orig_term_attr;
static struct termios new_term_attr;

void setupConsole(void)
{
}

void rawConsole(bool raw)
{
    if (raw)
    {
        tcgetattr(fileno(stdin), &orig_term_attr);
        memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
        new_term_attr.c_lflag &= ~(ECHO | ICANON);
        new_term_attr.c_cc[VTIME] = 0;
        new_term_attr.c_cc[VMIN] = 0;
        tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
    }
    else
    {
        tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
    }
}

void restoreConsole(void)
{
    printf("\x1b[0m");
}

void SetWindow(int Width, int Height)
{
    printf("\033[8;%d;%dt", Height, Width);
}
#endif

#endif

std::string ansi(ANSI_CODE code)
{
    std::string ret = "\x1b[";
    ret += std::to_string(code);
    ret += "m";
    return ret;
}

void clearConsole()
{
    std::cout << "\x1b[1;1H\x1b[2J";
}

void printDebug(std::string str, bool plain)
{
    if (print_debug)
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        if (!plain)
        {
            std::cout << ansi(BLUE_FG) << std::dec
                      << "[" << std::setfill('0') << std::setw(2) << ltm->tm_hour
                      << ":" << std::setfill('0') << std::setw(2) << ltm->tm_min
                      << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec
                      << "] " << str << ansi(RESET) << std::endl;
        }
        else
        {
            std::cout << ansi(BLUE_FG) << str << ansi(RESET);
        }
    }
}

void printInfo(std::string str)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::cout << ansi(WHITE_FG) << std::dec
              << "[" << std::setfill('0') << std::setw(2) << ltm->tm_hour
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_min
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec
              << "] " << str << ansi(RESET) << std::endl;
}

void printError(std::string str)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::cout << ansi(RED_FG) << std::dec
              << "[" << std::setfill('0') << std::setw(2) << ltm->tm_hour
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_min
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec
              << "] /!\\ ERROR /!\\ : " << str << ansi(RESET) << std::endl;
}

void printWarning(std::string str)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::cout << ansi(YELLOW_FG) << std::dec
              << "[" << std::setfill('0') << std::setw(2) << ltm->tm_hour
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_min
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec
              << "] ! Warning ! : " << str << ansi(RESET) << std::endl;
}
