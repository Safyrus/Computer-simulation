#pragma once
#include "assembler/Token.hpp"

class Node
{
protected:
    std::string type;
public:
    Node();
    virtual ~Node();

    virtual std::string toString();
    std::string getType();
    virtual Token getToken(int index);

    static const std::string NODE;
    static const std::string NODE_UNI;
    static const std::string NODE_BIN;
    static const std::string NODE_BINL;
    static const std::string NODE_MOV;
    static const std::string NODE_MOV2;
    static const std::string NODE_MOV2L;
    static const std::string NODE_MOVL;
    static const std::string NODE_TRI;
    static const std::string NODE_DUO;
};

