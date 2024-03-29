#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeMovL : public Node
{
private:
    Token cmd;
    Token reg;
    Token label;
public:
    NodeMovL(Token cmd, Token reg, Token label);
    virtual ~NodeMovL();

    virtual std::string toString();
    virtual Token getToken(int index);
};
