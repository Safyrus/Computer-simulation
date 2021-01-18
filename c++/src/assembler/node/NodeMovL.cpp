#include "assembler/node/NodeMovL.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeMovL::NodeMovL(Token cmd, Token reg, Token label)
{
    this->cmd = cmd;
    this->reg = reg;
    this->label = label;
    this->type = NODE_MOVL;
}

NodeMovL::~NodeMovL()
{
}

std::string NodeMovL::toString()
{
    return "(MOVL" + cmd.toString() + ", " + reg.toString()  + ", " + label.toString() + ")";
}

Token NodeMovL::getToken(int index)
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
        return label;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
