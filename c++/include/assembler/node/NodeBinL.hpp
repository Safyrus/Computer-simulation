#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeBinL : public Node
{
private:
    Token cmd;
    Token val;
    Token label;
public:
    NodeBinL(Token cmd, Token val, Token label);
    virtual ~NodeBinL();

    virtual std::string toString();
    virtual Token getToken(int index);
};
