#include "assembler/Token.hpp"
#include <iostream>
#include <string>

const std::string Token::HEX = "HEX";
const std::string Token::DEC = "DEC";
const std::string Token::CMD = "CMD";
const std::string Token::REG = "REG";
const std::string Token::LABEL = "LABEL";
const std::string Token::LABEL_DECLARE = "LABEL_DECLARE";
const std::string Token::COMMENT = "COMMENT";
const std::string Token::IMPORT = "IMPORT";
const std::string Token::ERR = "ERROR";


Token::Token()
{
    this->type = "";
    this->value = "";
    this->pos = Position();
}

Token::Token(std::string type, std::string value, Position pos)
{
    this->type = type;
    this->value = value;
    this->pos = pos;
}

Token::~Token()
{
}

void Token::print()
{
    std::cout << "[" << this->type << ":" << this->value << "]";
}

std::string Token::getValue()
{
    return this->value;
}

std::string Token::getType()
{
    return this->type;
}

Position Token::getPos()
{
    return this->pos;
}