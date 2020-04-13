#pragma once
#include <string>

class Position
{
private:
    int line;
    int col;
    int index;
    std::string fileName;
public:
    Position();
    Position(int line, int col, int index, std::string fileName);
    ~Position();

    void next(char c);

    int getLine();
    std::string getFileName();
    int getCol();
    int getIndex();

    Position copy();
};

