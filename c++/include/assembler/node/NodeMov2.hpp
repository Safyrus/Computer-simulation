#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeMov2 : public Node
{
private:
    Token cmd;
    Token reg;
    Token val1;
    Token val2;
public:
    NodeMov2(Token cmd, Token reg, Token val1, Token val2);
    virtual ~NodeMov2();

    virtual void print();
    virtual Token getToken(int index);
};
