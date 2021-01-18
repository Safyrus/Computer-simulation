#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeBin : public Node
{
private:
    Token cmd;
    Token val1;
    Token val2;
public:
    NodeBin(Token cmd, Token val1, Token val2);
    virtual ~NodeBin();

    virtual std::string toString();
    virtual Token getToken(int index);
};
