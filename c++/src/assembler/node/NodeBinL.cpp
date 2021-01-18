#include "assembler/node/NodeBinL.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeBinL::NodeBinL(Token cmd, Token val, Token label)
{
    this->cmd = cmd;
    this->val = val;
    this->label = label;
    this->type = NODE_BINL;
}

NodeBinL::~NodeBinL()
{
}

std::string NodeBinL::toString()
{
    return "(BINL" + cmd.toString() + ", " + val.toString() + ", " + label.toString() + ")";
}

Token NodeBinL::getToken(int index)
{
    if(index==0)
    {
        return cmd;
    }
    if(index==1)
    {
        return val;
    }
    if(index==2)
    {
        return label;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
