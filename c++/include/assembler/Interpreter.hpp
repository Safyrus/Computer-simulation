#pragma once
#include "assembler/node/Node.hpp"
#include <vector>

class Interpreter
{
private:
    std::vector<Node *> nodes;
    std::vector<Node *> labels;
    std::vector<Node *> externLabels;
    std::vector<int> labels_adr;
    std::vector<std::string> constants;
    std::vector<int> constants_val;
    static std::vector<std::string> importName;
    int index;

    static const std::vector<std::string> REG_NAME;
    static const std::vector<int> REG_CODE;
    static const std::vector<std::string> CMDS_NAME;
    static const std::vector<std::vector<int>> CMDS_CODE;

    std::string nodeUni(Node *n);
    std::string nodeBin(Node *n);
    std::string nodeMov(Node *n);
    std::string nodeMov2(Node *n);
    std::string nodeBinL(Node *n);
    std::string nodeMovL(Node *n);
    std::string nodeTri(Node *n);
    std::string nodeMov2L(Node *n);

    int getRegCode(Token reg);
    int getValCode(Token val);
    std::string getLabelVal(Token lab);
    std::vector<Node *> import(std::string fileName);

    void writeLabelFile(std::string fileName);

public:
    Interpreter(std::vector<Node *> nodes, std::string fileName);
    ~Interpreter();

    std::string interprete();
};