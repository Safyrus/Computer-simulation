#include "assembler/node/NodeUni.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeUni::NodeUni(Token token)
{
    this->type = NODE_UNI;
    this->token = token;
}

NodeUni::~NodeUni()
{
}

void NodeUni::print()
{
    std::cout << "(UNI";
    token.print();
    std::cout << ")";
}

Token NodeUni::getToken(int index)
{
    if(index==0)
    {
        return token;
    }
    return Token(Token::ERROR, "no token " + index, Position());
}