#include "assembler/node/NodeBin.hpp"
#include "assembler/Token.hpp"
#include <iostream>

NodeBin::NodeBin(Token cmd, Token val1, Token val2)
{
    this->cmd = cmd;
    this->val1 = val1;
    this->val2 = val2;
    this->type = NODE_BIN;
}

NodeBin::~NodeBin()
{
}

std::string NodeBin::toString()
{
    return "(BIN" + cmd.toString() + ", " + val1.toString() + ", " + val2.toString() + ")";
}

Token NodeBin::getToken(int index)
{
    if (index == 0)
    {
        return cmd;
    }
    if (index == 1)
    {
        return val1;
    }
    if (index == 2)
    {
        return val2;
    }
    return Token(Token::ERR, "no token " + index, Position());
}
