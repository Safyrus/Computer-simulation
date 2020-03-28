#pragma once
#include "assembler/Position.hpp"
#include <string>

class Token
{
private:
    std::string value;
    std::string type;
    Position pos;
public:
    Token();
    Token(std::string type, std::string value, Position pos);
    ~Token();

    static const std::string HEX;
    static const std::string DEC;
    static const std::string CMD;
    static const std::string REG;
    static const std::string LABEL;
    static const std::string LABEL_DECLARE;
    static const std::string COMMENT;
    static const std::string IMPORT;
    static const std::string ERROR;

    void print();

    std::string getValue();
    std::string getType();
    Position getPos();
};
