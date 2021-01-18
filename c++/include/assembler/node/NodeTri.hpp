#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeTri : public Node
{
private:
    Token cmd;
    Token val1;
    Token val2;
    Token val3;
public:
    NodeTri(Token cmd, Token val1, Token val2, Token val3);
    virtual ~NodeTri();

    virtual std::string toString();
    virtual Token getToken(int index);
};
