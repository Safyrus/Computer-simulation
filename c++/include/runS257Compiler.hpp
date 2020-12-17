#ifndef RUNS257COMPILER_HPP
#define RUNS257COMPILER_HPP

#include <string>
#include <vector>
#include <iostream>

#include "global.hpp"

#include "assembler/Lexer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Interpreter.hpp"

bool runS257Compiler(std::string filePath)
{
    Lexer *lexer;
    Parser *parser;
    Interpreter *interpreter;
    std::vector<Token> tokens;
    std::vector<Node *> nodes;
    bool error = false;
    std::string fileOut = "";
    std::string content = "";

    try
    {
        content = openFile(filePath);
    }
    catch (std::string e)
    {
        std::cout << e;
        return false;
    }

    //lexing
    lexer = new Lexer(content, filePath);
    tokens = lexer->makeToken();
    delete lexer;

    //verif lexing
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == Token::ERR)
        {
            error = true;
        }
        if (print_debug)
        {
            tokens[i].print();
            std::cout << ", ";
        }
    }
    if (print_debug)
        std::cout << "\n\n";
    if (error)
    {
        std::cout << "Error during Lexing, cannot continue\n"
                  << std::flush;
        return false;
    }

    //parsing
    parser = new Parser(tokens);
    nodes = parser->parse();
    delete parser;

    //print parsing
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (print_debug)
            nodes[i]->print();
        if (nodes[i]->getToken(0).getType().compare(Token::ERR) == 0)
        {
            error = true;
        }
        if (print_debug)
            std::cout << ", ";
    }
    if (print_debug)
        std::cout << "\n\n";
    if (error)
    {
        std::cout << "Error during Parsing, cannot continue\n"
                  << std::flush;
        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            delete nodes[i];
        }
        return false;
    }

    //interpreting
    interpreter = new Interpreter(nodes, filePath);
    fileOut = interpreter->interprete();
    delete interpreter;

    //verif interpreting
    std::cout << "\n\n"
              << std::flush;
    if (print_debug)
        std::cout << fileOut;

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        delete nodes[i];
    }
    error = fileOut.find("FFFFFFFF") != std::string::npos;
    if (error)
    {
        std::cout << "Error during Interpreting, cannot continue\n";
        return false;
    }
    writeFile(fileOut, filePath.substr(0, filePath.find_last_of('.')));

    return true;
}

#endif // RUNS257COMPILER_HPP