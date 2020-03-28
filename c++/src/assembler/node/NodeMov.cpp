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

void NodeMov::print()
{
    std::cout << "(MOV";
    cmd.print();
    std::cout << ", ";
    reg.print();
    std::cout << ", ";
    val.print();
    std::cout << ")";
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
    return Token(Token::ERROR, "no token " + index, Position());
}
