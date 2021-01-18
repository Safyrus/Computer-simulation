#include "assembler/node/NodeMov.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeMov::NodeMov(Token cmd, Token reg, Token val)
{
    this->cmd = cmd;
    this->reg = reg;
    this->val = val;
    this->type = NODE_MOV;
}

NodeMov::~NodeMov()
{
}

std::string NodeMov::toString()
{
    return "(MOV" + cmd.toString() + ", " + reg.toString() + ", " + val.toString() + ")";
}

Token NodeMov::getToken(int index)
{
    if(index==0)
    {
        return cmd;
    }
    if(index==1)
    {
        return reg;
    }
    if(index==2)
    {
        return val;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
