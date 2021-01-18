#include "assembler/Parser.hpp"
#include "assembler/node/NodeUni.hpp"
#include "assembler/node/NodeBin.hpp"
#include "assembler/node/NodeBinL.hpp"
#include "assembler/node/NodeMov.hpp"
#include "assembler/node/NodeMov2.hpp"
#include "assembler/node/NodeMovL.hpp"
#include "assembler/node/NodeTri.hpp"

#include "global.hpp"

#include "utils/console.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

const std::vector<std::string> Parser::CMDS_UNI = {"NOP", "RST", "OFF"};
const std::vector<std::string> Parser::CMDS_MOV = {"MOV"};
const std::vector<std::string> Parser::CMDS_BIN = {"CMP"};
const std::vector<std::string> Parser::CMDS_MOV2 = {"GET", "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR"};
const std::vector<std::string> Parser::CMDS_MOV2L = {"MOV"};
const std::vector<std::string> Parser::CMDS_MOVL = {"GET"};
const std::vector<std::string> Parser::CMDS_TRI = {"SET", "JMP"};
const std::vector<std::string> Parser::CMDS_BINL = {"SET", "JMP"};
const std::vector<std::string> Parser::CMDS_DOUBLE = {"SET", "GET", "JMP", "MOV"};

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->index = 0;
}

Parser::~Parser()
{
    this->index = 0;
}

void Parser::next()
{
    if (index != -1)
    {
        index++;
        if ((unsigned)index >= tokens.size())
        {
            index = -1;
        }
    }
}

std::vector<Node *> Parser::parse()
{
    std::vector<Node *> nodes;

    std::stringstream debugStr;

    while (index != -1)
    {
        Token tok = tokens[index];

        debugStr.str("");
        debugStr << std::setfill(' ') << std::setw(4) << index << " " << tok.getType() << " " << tok.getValue();
        printDebug(debugStr.str());

        if (tok.getType().compare(Token::CMD) != 0)
        {
            printDebug("     uni");
            nodes.push_back(new NodeUni(tok));
            next();
        }
        else
        {
            bool doubleNode = false;
            for (unsigned int i = 0; i < CMDS_DOUBLE.size(); i++)
            {
                if (tok.getValue().compare(CMDS_DOUBLE[i]) == 0)
                {
                    doubleNode = true;
                    break;
                }
            }
            if (doubleNode)
            {
                printDebug("     double");
                nodes.push_back(findDoubleNode());
            }
            else
            {
                printDebug("     not double");
                nodes.push_back(findCmdNode());
            }
        }
    }

    return nodes;
}

Node *Parser::findDoubleNode()
{
    Token cmd = tokens[index];
    std::vector<bool> boolean;
    int last = 7;
    boolean.resize(8);

    if (std::find(CMDS_UNI.begin(), CMDS_UNI.end(), cmd.getValue()) != CMDS_UNI.end())
    {
        boolean[0] = true;
        last = 0;
    }
    if (std::find(CMDS_BIN.begin(), CMDS_BIN.end(), cmd.getValue()) != CMDS_BIN.end())
    {
        boolean[1] = true;
        last = 1;
    }
    if (std::find(CMDS_BINL.begin(), CMDS_BINL.end(), cmd.getValue()) != CMDS_BINL.end())
    {
        boolean[2] = true;
        last = 2;
    }
    if (std::find(CMDS_MOV2L.begin(), CMDS_MOV2L.end(), cmd.getValue()) != CMDS_MOV2L.end())
    {
        boolean[3] = true;
        last = 3;
    }
    if (std::find(CMDS_MOV2.begin(), CMDS_MOV2.end(), cmd.getValue()) != CMDS_MOV2.end())
    {
        boolean[4] = true;
        last = 4;
    }
    if (std::find(CMDS_MOVL.begin(), CMDS_MOVL.end(), cmd.getValue()) != CMDS_MOVL.end())
    {
        boolean[5] = true;
        last = 5;
    }
    if (std::find(CMDS_MOV.begin(), CMDS_MOV.end(), cmd.getValue()) != CMDS_MOV.end())
    {
        boolean[6] = true;
        last = 6;
    }
    if (std::find(CMDS_TRI.begin(), CMDS_TRI.end(), cmd.getValue()) != CMDS_TRI.end())
    {
        boolean[7] = true;
        last = 7;
    }

    if (print_debug)
    {
        std::string debugStr = "";
        for (unsigned int i = 0; i < boolean.size(); i++)
        {
            debugStr += (boolean[i])?'1':'0';
        }
        printDebug("     " + debugStr);
    }

    if (std::find(boolean.begin(), boolean.end(), true) != boolean.end())
    {
        int i = 0;
        int tmpIndex = this->index;
        if (boolean[i])
        {
            next();
            return new NodeUni(cmd);
        }
        i++;
        if (boolean[i])
        {
            Node *n = makeCmdNodeBin((i == last));
            if (n->getToken(0).getType().compare(Token::ERR) != 0)
            {
                return n;
            }
            this->index = tmpIndex;
            delete n;
        }
        i++;
        if (boolean[i])
        {
            Node *n = makeCmdNodeBinL((i == last));
            if (n->getToken(0).getType().compare(Token::ERR) != 0)
            {
                return n;
            }
            this->index = tmpIndex;
            delete n;
        }
        i++;
        if (boolean[i])
        {
            Node *n = makeCmdNodeMov2L((i == last));
            if (n->getToken(0).getType().compare(Token::ERR) != 0)
            {
                return n;
            }
            this->index = tmpIndex;
            delete n;
        }
        i++;
        if (boolean[i])
        {
            Node *n = makeCmdNodeMov2((i == last));
            if (n->getToken(0).getType().compare(Token::ERR) != 0)
            {
                return n;
            }
            this->index = tmpIndex;
            delete n;
        }
        i++;
        if (boolean[i])
        {
            Node *n = makeCmdNodeMovL((i == last));
            if (n->getToken(0).getType().compare(Token::ERR) != 0)
            {
                return n;
            }
            this->index = tmpIndex;
            delete n;
        }
        i++;
        if (boolean[i])
        {
            Node *n = makeCmdNodeMov((i == last));
            if (n->getToken(0).getType().compare(Token::ERR) != 0)
            {
                return n;
            }
            this->index = tmpIndex;
            delete n;
        }
        return makeCmdNodeTri(true);
    }
    else
    {
        printError("[ASSEMBLER ERROR]: Unknow Token in file " + cmd.getPos().getFileName() + " at " + std::to_string(cmd.getPos().getLine()) + ":" + std::to_string(cmd.getPos().getCol()));
        next();
        return new NodeUni(Token(Token::ERR, "Unknow Token", cmd.getPos()));
    }
}

Node *Parser::findCmdNode()
{
    Token cmd = tokens[index];

    if (std::find(CMDS_UNI.begin(), CMDS_UNI.end(), cmd.getValue()) != CMDS_UNI.end())
    {
        next();
        return new NodeUni(cmd);
    }
    else if (std::find(CMDS_BIN.begin(), CMDS_BIN.end(), cmd.getValue()) != CMDS_BIN.end())
    {
        return makeCmdNodeBin(true);
    }
    else if (std::find(CMDS_BINL.begin(), CMDS_BINL.end(), cmd.getValue()) != CMDS_BINL.end())
    {
        return makeCmdNodeBinL(true);
    }
    else if (std::find(CMDS_MOV2.begin(), CMDS_MOV2.end(), cmd.getValue()) != CMDS_MOV2.end())
    {
        return makeCmdNodeMov2(true);
    }
    else if (std::find(CMDS_MOVL.begin(), CMDS_MOVL.end(), cmd.getValue()) != CMDS_MOVL.end())
    {
        return makeCmdNodeMovL(true);
    }
    else if (std::find(CMDS_MOV2L.begin(), CMDS_MOV2L.end(), cmd.getValue()) != CMDS_MOV2L.end())
    {
        return makeCmdNodeMov2L(true);
    }
    else if (std::find(CMDS_MOV.begin(), CMDS_MOV.end(), cmd.getValue()) != CMDS_MOV.end())
    {
        return makeCmdNodeMov(true);
    }
    else if (std::find(CMDS_TRI.begin(), CMDS_TRI.end(), cmd.getValue()) != CMDS_TRI.end())
    {
        return makeCmdNodeTri(true);
    }
    else
    {
        printError("[ASSEMBLER ERROR]: Unknow Token in file " + cmd.getPos().getFileName() + " at " + std::to_string(cmd.getPos().getLine()) + ":" + std::to_string(cmd.getPos().getCol()));
        next();
        return new NodeUni(Token(Token::ERR, "Unknow Token", cmd.getPos()));
    }
}

NodeBin *Parser::makeCmdNodeBin(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " bin";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBin(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token val1 = tokens[index];
    if (val1.getType() != Token::DEC && val1.getType() != Token::HEX && val1.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected hex, dec or reg in file " << val1.getPos().getFileName() << " at " << val1.getPos().getLine() << ":" << val1.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBin(Token(Token::ERR, "argument type incorrect", val1.getPos()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBin(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token val2 = tokens[index];
    if (val2.getType() != Token::DEC && val2.getType() != Token::HEX && val2.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg in file " << val2.getPos().getFileName() << " at " << val2.getPos().getLine() << ":" << val2.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBin(Token(Token::ERR, "argument type incorrect", val2.getPos()), Token(), Token());
    }
    next();
    return new NodeBin(cmd, val1, val2);
}

NodeMov *Parser::makeCmdNodeMov(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " mov";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token reg = tokens[index];
    if (reg.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg in file " << reg.getPos().getFileName() << " at " << reg.getPos().getLine() << ":" << reg.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov(Token(Token::ERR, "argument type incorrect", reg.getPos()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token val = tokens[index];
    if (val.getType() != Token::DEC && val.getType() != Token::HEX && val.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg in file " << val.getPos().getFileName() << " at " << val.getPos().getLine() << ":" << val.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov(Token(Token::ERR, "argument type incorrect", val.getPos()), Token(), Token());
    }
    next();
    return new NodeMov(cmd, reg, val);
}

NodeMov2 *Parser::makeCmdNodeMov2(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " mov2";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 3 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token reg = tokens[index];
    if (reg.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg in file " << reg.getPos().getFileName() << " at " << reg.getPos().getLine() << ":" << reg.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2(Token(Token::ERR, "argument type incorrect", reg.getPos()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token val1 = tokens[index];
    if (val1.getType() != Token::DEC && val1.getType() != Token::HEX && val1.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg in file " << val1.getPos().getFileName() << " at " << val1.getPos().getLine() << ":" << val1.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2(Token(Token::ERR, "argument type incorrect", val1.getPos()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token val2 = tokens[index];
    if (val2.getType() != Token::DEC && val2.getType() != Token::HEX && val2.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 3 incorrect, expected hex, dec or reg in file " << val2.getPos().getFileName() << " at " << val2.getPos().getLine() << ":" << val2.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2(Token(Token::ERR, "argument type incorrect", val2.getPos()), Token(), Token(), Token());
    }
    next();
    return new NodeMov2(cmd, reg, val1, val2);
}

NodeBinL *Parser::makeCmdNodeBinL(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " binL";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBinL(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token val = tokens[index];
    if (val.getType() != Token::DEC && val.getType() != Token::HEX && val.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected hex, dec or reg in file " << val.getPos().getFileName() << " at " << val.getPos().getLine() << ":" << val.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBinL(Token(Token::ERR, "argument type incorrect", val.getPos()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBinL(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token label = tokens[index];
    if (label.getType() != Token::LABEL)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeBinL(Token(Token::ERR, "argument type incorrect", label.getPos()), Token(), Token());
    }
    next();
    return new NodeBinL(cmd, val, label);
}

NodeMovL *Parser::makeCmdNodeMovL(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " movL";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMovL(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token reg = tokens[index];
    if (reg.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg in file " << reg.getPos().getFileName() << " at " << reg.getPos().getLine() << ":" << reg.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMovL(Token(Token::ERR, "argument type incorrect", reg.getPos()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMovL(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token());
    }
    Token label = tokens[index];
    if (label.getType() != Token::LABEL)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMovL(Token(Token::ERR, "argument type incorrect", label.getPos()), Token(), Token());
    }
    next();
    return new NodeMovL(cmd, reg, label);
}

NodeTri *Parser::makeCmdNodeTri(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " tri";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 3 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeTri(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token val1 = tokens[index];
    if (val1.getType() != Token::DEC && val1.getType() != Token::HEX && val1.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected hex, dec or reg in file " << val1.getPos().getFileName() << " at " << val1.getPos().getLine() << ":" << val1.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeTri(Token(Token::ERR, "argument type incorrect", val1.getPos()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeTri(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token val2 = tokens[index];
    if (val2.getType() != Token::DEC && val2.getType() != Token::HEX && val2.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg in file " << val2.getPos().getFileName() << " at " << val2.getPos().getLine() << ":" << val2.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeTri(Token(Token::ERR, "argument type incorrect", val2.getPos()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeTri(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token val3 = tokens[index];
    if (val3.getType() != Token::DEC && val3.getType() != Token::HEX && val3.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 3 incorrect, expected hex, dec or reg in file " << val3.getPos().getFileName() << " at " << val3.getPos().getLine() << ":" << val3.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeTri(Token(Token::ERR, "argument type incorrect", val3.getPos()), Token(), Token(), Token());
    }
    next();
    return new NodeTri(cmd, val1, val2, val3);
}

NodeMov2L *Parser::makeCmdNodeMov2L(bool print)
{
    std::stringstream debugStr;
    debugStr << std::setfill(' ') << std::setw(4) << index << " mov2L";
    printDebug(debugStr.str());
    debugStr.str("");

    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 3 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2L(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token reg1 = tokens[index];
    if (reg1.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg in file " << reg1.getPos().getFileName() << " at " << reg1.getPos().getLine() << ":" << reg1.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2L(Token(Token::ERR, "argument type incorrect", reg1.getPos()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 2 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2L(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token reg2 = tokens[index];
    if (reg2.getType() != Token::REG)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected reg in file " << reg2.getPos().getFileName() << " at " << reg2.getPos().getLine() << ":" << reg2.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2L(Token(Token::ERR, "argument type incorrect", reg2.getPos()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: Missing 1 argument in file " << cmd.getPos().getFileName() << " at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2L(Token(Token::ERR, "Missing argument", cmd.getPos()), Token(), Token(), Token());
    }
    Token label = tokens[index];
    if (label.getType() != Token::LABEL)
    {
        if (print)
        {
            debugStr << "[ASSEMBLER ERROR]: type of argument 3 incorrect, expected label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol();
            printError(debugStr.str());
        }
        return new NodeMov2L(Token(Token::ERR, "argument type incorrect", label.getPos()), Token(), Token(), Token());
    }
    next();
    return new NodeMov2L(cmd, reg1, reg2, label);
}
