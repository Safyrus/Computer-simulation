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

std::string NodeUni::toString()
{
    return "(UNI" + token.toString() + ")";
}

Token NodeUni::getToken(int index)
{
    if(index==0)
    {
        return token;
    }
    return Token(Token::ERR, "no token " + index, Position());
}