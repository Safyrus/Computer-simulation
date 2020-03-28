#pragma once
#include <string>

class Position
{
private:
    int line;
    int col;
    int index;
public:
    Position();
    Position(int line, int col, int index);
    ~Position();

    void next(char c);

    int getLine();
    int getCol();
    int getIndex();

    Position copy();
};

