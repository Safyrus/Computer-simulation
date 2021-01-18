#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeMov : public Node
{
private:
    Token cmd;
    Token reg;
    Token val;
public:
    NodeMov(Token cmd, Token reg, Token val);
    virtual ~NodeMov();

    virtual std::string toString();
    virtual Token getToken(int index);
};
