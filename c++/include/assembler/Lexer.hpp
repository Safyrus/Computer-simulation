#pragma once
#include <string>
#include <vector>
#include "assembler/Token.hpp"
#include "assembler/Position.hpp"

class Lexer
{
private:
    std::string text;
    std::string fileName;
    Position pos;
    char current_char;

    static const char CHAR_IMPORT = '@';
    static const char CHAR_DEC = '%';
    static const char CHAR_REG = '$';
    static const char CHAR_COMMENT = '#';
    static const char CHAR_LABEL = ':';
    static const std::vector<std::string> CMDS;

    void next();

    Token makeComment();
    Token makeDecimal();
    Token makeImport();
    Token makeLabel();
    Token makeReg();
    Token findToken();

    std::string findWord();
public:
    Lexer(std::string text, std::string fileName);
    ~Lexer();

    std::vector<Token> makeToken();
};
