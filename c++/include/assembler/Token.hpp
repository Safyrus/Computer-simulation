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
    static const std::string BIN;
    static const std::string CHAR;
    static const std::string STRING;
    static const std::string CMD;
    static const std::string REG;
    static const std::string LABEL;
    static const std::string LABEL_LOW;
    static const std::string LABEL_HIGH;
    static const std::string LABEL_DECLARE;
    static const std::string COMMENT;
    static const std::string IMPORT;
    static const std::string ORIGIN;
    static const std::string CONSTANT_DECLARE;
    static const std::string CONSTANT;
    static const std::string EXTERN;
    static const std::string ERR;

    std::string toString();

    std::string getValue();
    std::string getType();
    Position getPos();
};
