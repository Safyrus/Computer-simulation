#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeMov2L : public Node
{
private:
    Token cmd;
    Token reg1;
    Token reg2;
    Token label;
public:
    NodeMov2L(Token cmd, Token reg1, Token reg2, Token label);
    virtual ~NodeMov2L();

    virtual std::string toString();
    virtual Token getToken(int index);
};
