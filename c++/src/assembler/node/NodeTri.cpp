#include "assembler/node/NodeTri.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeTri::NodeTri(Token cmd, Token val1, Token val2, Token val3)
{
    this->cmd = cmd;
    this->val1 = val1;
    this->val2 = val2;
    this->val3 = val3;
    this->type = NODE_TRI;
}

NodeTri::~NodeTri()
{
}

std::string NodeTri::toString()
{
    return "(TRI" + cmd.toString() + ", " + val1.toString() + ", " + val2.toString() + ", " + val3.toString() + ")";
}

Token NodeTri::getToken(int index)
{
    if(index==0)
    {
        return cmd;
    }
    if(index==1)
    {
        return val1;
    }
    if(index==2)
    {
        return val2;
    }
    if(index==3)
    {
        return val3;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
