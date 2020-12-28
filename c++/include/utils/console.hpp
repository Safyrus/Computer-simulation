#pragma once

#include <string>

enum ANSI_CODE
{
    RESET = 0,
    BOLD = 1,
    FAINT = 2,
    ITALIC = 3,
    UNDERLINE = 4,
    BLACK_FG = 30,
    RED_FG = 31,
    GREEN_FG = 32,
    YELLOW_FG = 33,
    BLUE_FG = 34,
    MAGENTA_FG = 35,
    CYAN_FG = 36,
    WHITE_FG = 37,
    DEFAULT_FG = 39,
    BLACK_BG = 40,
    RED_BG = 41,
    GREEN_BG = 42,
    YELLOW_BG = 43,
    BLUE_BG = 44,
    MAGENTA_BG = 45,
    CYAN_BG = 46,
    WHITE_BG = 47,
    DEFAULT_BG = 49
};

void setupConsole(void);
void restoreConsole(void);
void SetWindow(int Width, int Height);
void rawConsole(bool raw);

std::string ansi(ANSI_CODE code);
