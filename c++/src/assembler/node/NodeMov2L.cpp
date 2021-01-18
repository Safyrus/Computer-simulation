#include "assembler/node/NodeMov2L.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeMov2L::NodeMov2L(Token cmd, Token reg1, Token reg2, Token label)
{
    this->cmd = cmd;
    this->reg1 = reg1;
    this->reg2 = reg2;
    this->label = label;
    this->type = NODE_MOV2L;
}

NodeMov2L::~NodeMov2L()
{
}

std::string NodeMov2L::toString()
{
    return "(MOV2L" + cmd.toString() + ", " + reg1.toString() + ", " + reg2.toString() + ", " + label.toString() + ")";
}

Token NodeMov2L::getToken(int index)
{
    if(index==0)
    {
        return cmd;
    }
    if(index==1)
    {
        return reg1;
    }
    if(index==2)
    {
        return reg2;
    }
    if(index==3)
    {
        return label;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
