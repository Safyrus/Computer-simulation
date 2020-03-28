#include "assembler/Position.hpp"
#include <string>

Position::Position()
{
    this->index = -1;
    this->col = 0;
    this->line = 1;
}

Position::Position(int line, int col, int index)
{
    this->index = index;
    this->col = col;
    this->line = line;
}

Position::~Position()
{
}

void Position::next(char c)
{
    col++;
    index++;

    if (c == '\n')
    {
        line++;
        col = 0;
    }
}

int Position::getLine()
{
    return line;
}

int Position::getCol()
{
    return col;
}

int Position::getIndex()
{
    return index;
}

Position Position::copy()
{
    return Position(line, col, index);
}