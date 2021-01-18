#include "assembler/node/Node.hpp"

const std::string Node::NODE = "NODE";
const std::string Node::NODE_UNI = "NODE_UNI";
const std::string Node::NODE_BIN = "NODE_BIN";
const std::string Node::NODE_BINL = "NODE_BINL";
const std::string Node::NODE_MOV = "NODE_MOV";
const std::string Node::NODE_MOV2 = "NODE_MOV2";
const std::string Node::NODE_MOVL = "NODE_MOVL";
const std::string Node::NODE_TRI = "NODE_TRI";
const std::string Node::NODE_MOV2L = "NODE_MOV2L";

Node::Node()
{
    type = NODE;
}

Node::~Node()
{
}

std::string Node::toString()
{
    return "";
}

Token Node::getToken(int index)
{
    return Token(Token::ERR, "no token " + index, Position());
}

std::string Node::getType()
{
    return type;
}