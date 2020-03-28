#pragma once
#include "assembler/node/Node.hpp"
#include <vector>

class Interpreter
{
private:
    std::vector<Node*> nodes;
    std::vector<Node*> labels;
    std::vector<int> labels_adr;
    static std::vector<std::string> importName;
    int index;

    static const std::vector<std::string> REG_NAME;
    static const std::vector<int> REG_CODE;
    static const std::vector<std::string> CMDS_NAME;
    static const std::vector<std::vector<int>> CMDS_CODE;

    std::string nodeUni(Node* n);
    std::string nodeBin(Node* n);
    std::string nodeMov(Node* n);
    std::string nodeMov2(Node* n);
    std::string nodeBinL(Node* n);
    std::string nodeMovL(Node*n);
    std::string nodeTri(Node* n);

    int getRegCode(Token reg);
    int getValCode(Token val);
    std::string import(std::string fileName);
public:
    Interpreter(std::vector<Node*> nodes, std::string fileName);
    ~Interpreter();

    std::string interprete();
};