#include "assembler/node/NodeDuo.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeDuo::NodeDuo(Token token, Token val)
{
    this->type = NODE_DUO;
    this->token = token;
    this->val = val;
}

NodeDuo::~NodeDuo()
{
}

std::string NodeDuo::toString()
{
    return "(DUO" + token.toString() + ", " + val.toString() + ")";
}

Token NodeDuo::getToken(int index)
{
    if (index == 0)
    {
        return token;
    }
    if (index == 1)
    {
        return val;
    }
    return Token(Token::ERR, "no token " + index, Position());
}