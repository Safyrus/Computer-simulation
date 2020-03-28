#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeUni : public Node
{
private:
    Token token;
public:
    NodeUni();
    NodeUni(Token token);
    virtual ~NodeUni();

    virtual void print();
    virtual Token getToken(int index);
};
