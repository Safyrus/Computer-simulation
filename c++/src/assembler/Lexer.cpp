#include "assembler/Lexer.hpp"

#include "utils/console.hpp"

#include <iostream>
#include <string.h>

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
        else if (current_char == CHAR_IMPORT)
        {
            tokens.push_back(makeImport());
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

Token Lexer::makeImport()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::IMPORT, word, newPos);
}

Token Lexer::makeLabel()
{
    Position newPos = pos.copy();
    next();
    std::string word = findWord();
    return Token(Token::LABEL, word, newPos);
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

    std::string error = "[ASSEMBLER ERROR]: Undefined word " + word + " in file " + fileName + " at " + std::to_string(newPos.getLine()) + ":" + std::to_string(newPos.getCol());
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