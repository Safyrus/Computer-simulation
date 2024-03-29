#pragma once
#include "assembler/Token.hpp"
#include "assembler/node/Node.hpp"
#include "assembler/node/NodeBin.hpp"
#include "assembler/node/NodeBinL.hpp"
#include "assembler/node/NodeMov.hpp"
#include "assembler/node/NodeMov2.hpp"
#include "assembler/node/NodeMovL.hpp"
#include "assembler/node/NodeTri.hpp"
#include "assembler/node/NodeMov2L.hpp"
#include "assembler/node/NodeDuo.hpp"
#include <vector>

class Parser
{
private:
    std::vector<Token> tokens;
    int index;
    static const std::vector<std::string> CMDS_UNI;
    static const std::vector<std::string> CMDS_MOV;
    static const std::vector<std::string> CMDS_BIN;
    static const std::vector<std::string> CMDS_MOV2;
    static const std::vector<std::string> CMDS_MOVL;
    static const std::vector<std::string> CMDS_TRI;
    static const std::vector<std::string> CMDS_MOV2L;
    static const std::vector<std::string> CMDS_BINL;
    static const std::vector<std::string> CMDS_DOUBLE;

    void next();
    bool checkNext(bool print, int argNum);
    bool checkVal(Token val, bool print, int argNum);
    bool checkReg(Token reg, bool print, int argNum);
    bool checkLabel(Token label, bool print, int argNum);

    Node* findDoubleNode();
    Node* findCmdNode();
    NodeBin* makeCmdNodeBin(bool print);
    NodeMov* makeCmdNodeMov(bool print);
    NodeMov2* makeCmdNodeMov2(bool print);
    NodeBinL* makeCmdNodeBinL(bool print);
    NodeMovL* makeCmdNodeMovL(bool print);
    NodeTri* makeCmdNodeTri(bool print);
    NodeMov2L* makeCmdNodeMov2L(bool print);
    NodeDuo* makeNodeDuo(bool print);
public:
    Parser(std::vector<Token> tokens);
    ~Parser();

    std::vector<Node*> parse();
};