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

    std::vector<std::string> constants;

    void next();

    Token makeComment();
    Token makeDecimal();
    Token makeBinary();
    Token makeChar();
    Token makeString();
    Token makeImport();
    Token makeOrigin();
    Token makeConstDeclare();
    Token makeExternDeclare();
    Token makeConst();
    Token makeLabel();
    Token makeReg();
    Token findToken();
    Token findAssemblerCmd();

    std::string findWord();
    std::string findChar();
    std::string findStr();

public:
    Lexer(std::string text, std::string fileName);
    ~Lexer();

    static const char CHAR_ASSEMBLER = '@';
    static const char CHAR_DEC = '%';
    static const char CHAR_BIN = '~';
    static const char CHAR_CHAR = '\'';
    static const char CHAR_STRING = '"';
    static const char CHAR_REG = '$';
    static const char CHAR_COMMENT = '#';
    static const char CHAR_LABEL = ':';
    static const char CHAR_LABEL_LOW = 'l';
    static const char CHAR_LABEL_HIGH = 'h';
    static const std::vector<std::string> CMDS;

    std::vector<Token> makeToken();
};
