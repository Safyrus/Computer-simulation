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

void NodeMovL::print()
{
    std::cout << "(MOVL";
    cmd.print();
    std::cout << ", ";
    reg.print();
    std::cout << ", ";
    label.print();
    std::cout << ")";
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
    return Token(Token::ERROR, "no token " + index, Position());
}
