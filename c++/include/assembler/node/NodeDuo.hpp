#pragma once

#include "assembler/node/Node.hpp"
#include "assembler/Token.hpp"

class NodeDuo : public Node
{
private:
    Token token;
    Token val;
public:
    NodeDuo();
    NodeDuo(Token token, Token val);
    virtual ~NodeDuo();

    virtual std::string toString();
    virtual Token getToken(int index);
};
