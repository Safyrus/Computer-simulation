#include "assembler/Lexer.hpp"

#include "utils/console.hpp"

#include <iostream>
#include <string.h>
#include <locale>
#include <codecvt>

const std::vector<std::string> Lexer::CMDS = {"NOP", "RST", "OFF", "MOV", "CMP", "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR", "JMP", "GET", "SET"};

Lexer::Lexer(std::string text, std::string fileName)
{
    this->fileName = fileName;
    this->text = text;
    this->pos = Position(1, 0, -1, fileName);
    this->current_char = '\0';
    this->next();
}

Lexer::~Lexer()
{
}

void Lexer::next()
{
    pos.next(current_char);
    if ((unsigned)pos.getIndex() < text.size())
    {
        current_char = text[pos.getIndex()];
    }
    else
    {
        current_char = '\0';
    }
}

std::vector<Token> Lexer::makeToken()
{
    this->pos = Position(1, 0, -1, fileName);
    this->current_char = '\0';
    this->next();
    std::vector<Token> tokens;

    while (current_char != '\0')
    {
        if (current_char == '\t' || current_char == ' ' || current_char == '\n')
        {
            next();
        }
        else if (current_char == CHAR_COMMENT)
        {
            tokens.push_back(makeComment());
        }
        else if (current_char == CHAR_REG)
        {
            tokens.push_back(makeReg());
        }
        else if (current_char == CHAR_DEC)
        {
            tokens.push_back(makeDecimal());
        }
        else if (current_char == CHAR_BIN)
        {
            tokens.push_back(makeBinary());
        }
        else if (current_char == CHAR_CHAR)
        {
            tokens.push_back(makeChar());
        }
        else if (current_char == CHAR_STRING)
        {
            tokens.push_back(makeString());
        }
        else if (current_char == CHAR_ASSEMBLER)
        {
            tokens.push_back(findAssemblerCmd());
        }
        else if (current_char == CHAR_LABEL)
        {
            tokens.push_back(makeLabel());
        }
        else
        {
            tokens.push_back(findToken());
        }
    }
    return tokens;
}

Token Lexer::makeComment()
{
    Position newPos = pos.copy();
    next();
    std::string word = "";
    while (current_char != '\n')
    {
        word += current_char;
        next();
    }
    return Token(Token::COMMENT, word, newPos);
}

Token Lexer::makeDecimal()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::DEC, word, newPos);
}

Token Lexer::makeBinary()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::BIN, word, newPos);
}

Token Lexer::makeChar()
{
    Position newPos = pos.copy();
    next();
    std::string word = findChar();
    if (word.back() != CHAR_CHAR)
    {
        std::string error = "[ASSEMBLER ERROR]: Char type " + word.substr(0, 1) + " need a " + CHAR_CHAR + " at the end in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
        printError(error);
        return Token(Token::ERR, word, newPos);
    }
    word = word.substr(0, word.size() - 1);
    if (word.size() > 1)
    {
        std::string error = "[ASSEMBLER ERROR]: Multiple character (" + word + ") for char declaration in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
        printError(error);
        return Token(Token::ERR, word, newPos);
    }
    return Token(Token::CHAR, word, newPos);
}

Token Lexer::makeString()
{
    Position newPos = pos.copy();
    next();
    std::string word = findStr();
    if (word.back() != CHAR_STRING)
    {
        std::string error = "[ASSEMBLER ERROR]: String type " + word.substr(0, word.size() - 1) + " need a " + CHAR_STRING + " at the end in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
        printError(error);
        return Token(Token::ERR, word, newPos);
    }
    word = word.substr(0, word.size() - 1);
    std::string newWord = "";
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring str = converter.from_bytes(word);
    for (unsigned int i = 0; i < str.size(); i++)
    {
        char valStr = str[i];
        newWord += valStr;
    }
    return Token(Token::STRING, newWord, newPos);
}

Token Lexer::makeImport()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::IMPORT, word, newPos);
}

Token Lexer::makeOrigin()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::ORIGIN, word, newPos);
}

Token Lexer::makeConstDeclare()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();

    for (unsigned int i = 0; i < constants.size(); i++)
    {
        if(constants[i].compare(word) == 0)
        {
            std::string error = "[ASSEMBLER ERROR]: Cannot declare the constant " + word + " multiple time in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
            printError(error);
            return Token(Token::ERR, word, newPos);
        }
    }
    constants.push_back(word);

    return Token(Token::CONSTANT_DECLARE, word, newPos);
}

Token Lexer::makeConst()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::CONSTANT, word, newPos);
}

Token Lexer::makeLabel()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    if (word[word.size() - 2] == CHAR_LABEL)
    {
        if (word.back() == CHAR_LABEL_LOW)
        {
            return Token(Token::LABEL_LOW, word.substr(0, word.size() - 2), newPos);
        }
        else if (word.back() == CHAR_LABEL_HIGH)
        {
            return Token(Token::LABEL_HIGH, word.substr(0, word.size() - 2), newPos);
        }
        else
        {
            std::string error = "[ASSEMBLER ERROR]: Undefined type " + word.substr(word.size() - 2, word.size() - 1) + " for label " + word.substr(0, word.size() - 2) + " in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
            printError(error);
            return Token(Token::ERR, word, newPos);
        }
    }
    else
    {
        return Token(Token::LABEL, word, newPos);
    }
}

Token Lexer::makeReg()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::REG, word, newPos);
}

Token Lexer::findToken()
{
    Position newPos = pos.copy();
    std::string word = findWord();

    if (word.back() == CHAR_LABEL)
    {
        word = word.substr(0, word.size() - 1);
        return Token(Token::LABEL_DECLARE, word, newPos);
    }

    for (unsigned int i = 0; i < CMDS.size(); i++)
    {
        if (CMDS[i].compare(word) == 0)
        {
            return Token(Token::CMD, word, newPos);
        }
    }

    if (word.c_str()[strspn(word.c_str(), "0123456789abcdefABCDEF")] == 0)
    {
        return Token(Token::HEX, word, newPos);
    }

    for (unsigned int i = 0; i < constants.size(); i++)
    {
        if(constants[i].compare(word) == 0)
        {
            return Token(Token::CONSTANT, word, newPos);
        }
    }
    std::string warning = "[ASSEMBLER WARNING]: Undefined word " + word + " in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol()) + ". Try to use undeclare constant";
    printWarning(warning);
    return Token(Token::CONSTANT, word, newPos);

    /*
    std::string error = "[ASSEMBLER ERROR]: Undefined word " + word + " in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
    printError(error);
    return Token(Token::ERR, word, newPos);
    */
}

Token Lexer::findAssemblerCmd()
{
    Position newPos = pos.copy();
    std::string word = findWord();

    if (word.compare("@import") == 0)
    {
        return makeImport();
    }
    else if (word.compare("@origin") == 0)
    {
        return makeOrigin();
    }
    else if (word.compare("@const") == 0)
    {
        return makeConstDeclare();
    }

    std::string error = "[ASSEMBLER ERROR]: Undefined assembler command " + word + " in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
    printError(error);
    return Token(Token::ERR, word, newPos);
}

std::string Lexer::findWord()
{
    std::string word = "";
    while (current_char != '\t' && current_char != ' ' && current_char != '\n' && current_char != Lexer::CHAR_COMMENT)
    {
        word += current_char;
        next();
    }
    return word;
}

std::string Lexer::findChar()
{
    std::string strChar = "";
    strChar += current_char;
    next();
    strChar += current_char;
    next();
    return strChar;
}

std::string Lexer::findStr()
{
    std::string str = "";
    while (current_char != CHAR_STRING && current_char != '\0')
    {
        str += current_char;
        next();
    }
    str += current_char;
    next();
    return str;
}
