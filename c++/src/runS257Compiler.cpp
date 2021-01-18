#include "runS257Compiler.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "global.hpp"

#include "utils/console.hpp"

#include "assembler/Lexer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Interpreter.hpp"

bool runS257Compiler(std::string filePath)
{
    // declare variables
    Lexer *lexer;
    Parser *parser;
    Interpreter *interpreter;
    std::vector<Token> tokens;
    std::vector<Node *> nodes;
    bool error = false;
    std::string fileOut = "";
    std::string content = "";
    std::string debugStr = "";

    // read the file
    try
    {
        content = readFile(filePath);
    }
    catch (std::string e)
    {
        printError(e);
        return false;
    }

    //lexing
    lexer = new Lexer(content, filePath);
    tokens = lexer->makeToken();
    delete lexer;

    //verif lexing
    debugStr.clear();
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == Token::ERR)
        {
            error = true;
        }
        if (print_debug)
        {
            debugStr += tokens[i].toString() + ", ";
        }
    }
    printDebug(debugStr + "\n");
    if (error)
    {
        printError("Error during Lexing, cannot continue\n");
        return false;
    }

    //parsing
    parser = new Parser(tokens);
    nodes = parser->parse();
    delete parser;

    //print parsing
    debugStr.clear();
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (print_debug)
            debugStr += nodes[i]->toString() + ", ";
        if (nodes[i]->getToken(0).getType().compare(Token::ERR) == 0)
        {
            error = true;
        }
    }
    printDebug(debugStr + "\n");
    if (error)
    {
        printError("Error during Parsing, cannot continue\n");
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
    printDebug("\n" + fileOut);
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        delete nodes[i];
    }
    error = fileOut.find("FFFFFFFF") != std::string::npos;
    if (error)
    {
        printError("Error during Interpreting, cannot continue\n");
        return false;
    }
    writeHexFile(fileOut, filePath.substr(0, filePath.find_last_of('.')));

    return true;
}
