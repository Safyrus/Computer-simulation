#include "assembler/node/NodeMov2.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeMov2::NodeMov2(Token cmd, Token reg, Token val1, Token val2)
{
    this->cmd = cmd;
    this->reg = reg;
    this->val1 = val1;
    this->val2 = val2;
    this->type = NODE_MOV2;
}

NodeMov2::~NodeMov2()
{
}

std::string NodeMov2::toString()
{
    return "(MOV2" + cmd.toString() + ", " + reg.toString() + ", " + val1.toString() + ", " + val2.toString() + ")";
}

Token NodeMov2::getToken(int index)
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
        return val1;
    }
    if(index==3)
    {
        return val2;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
