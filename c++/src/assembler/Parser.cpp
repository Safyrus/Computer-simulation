#include "assembler/Parser.hpp"
#include "assembler/node/NodeUni.hpp"
#include "assembler/node/NodeBin.hpp"
#include "assembler/node/NodeBinL.hpp"
#include "assembler/node/NodeMov.hpp"
#include "assembler/node/NodeMov2.hpp"
#include "assembler/node/NodeMovL.hpp"
#include "assembler/node/NodeTri.hpp"
#include <iostream>
#include <algorithm>
#include "global.hpp"

const std::vector<std::string> Parser::CMDS_UNI = {"NOP", "RST", "OFF"};
const std::vector<std::string> Parser::CMDS_MOV = {"MOV"};
const std::vector<std::string> Parser::CMDS_BIN = {"CMP"};
const std::vector<std::string> Parser::CMDS_MOV2 = {"GET", "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR"};
const std::vector<std::string> Parser::CMDS_MOVL = {"GET"};
const std::vector<std::string> Parser::CMDS_TRI = {"SET", "JMP"};
const std::vector<std::string> Parser::CMDS_BINL = {"SET", "JMP"};
const std::vector<std::string> Parser::CMDS_DOUBLE = {"SET", "GET", "JMP"};

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

    while (index != -1)
    {
        Token tok = tokens[index];
        if (tok.getType().compare(Token::CMD) != 0)
        {
            if (print_debug)
                std::cout << index << "uni \n";
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
                if (print_debug)
                    std::cout << index << "double \n";
                nodes.push_back(findDoubleNode());
            }
            else
            {
                if (print_debug)
                    std::cout << index << " val: " << tok.getValue() << " not double \n";
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
    int last = 6;
    boolean.resize(7);

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
    if (std::find(CMDS_MOV.begin(), CMDS_MOV.end(), cmd.getValue()) != CMDS_MOV.end())
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
    if (std::find(CMDS_TRI.begin(), CMDS_TRI.end(), cmd.getValue()) != CMDS_TRI.end())
    {
        boolean[6] = true;
        last = 6;
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
            if (n->getToken(0).getType().compare(Token::ERROR) != 0)
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
            if (n->getToken(0).getType().compare(Token::ERROR) != 0)
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
            if (n->getToken(0).getType().compare(Token::ERROR) != 0)
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
            if (n->getToken(0).getType().compare(Token::ERROR) != 0)
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
            if (n->getToken(0).getType().compare(Token::ERROR) != 0)
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
        std::cout << "[ASSEMBLER ERROR]: Unknow Token\n";
        next();
        return new NodeUni(Token(Token::ERROR, "Unknow Token", Position()));
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
    else if (std::find(CMDS_MOV.begin(), CMDS_MOV.end(), cmd.getValue()) != CMDS_MOV.end())
    {
        return makeCmdNodeMov(true);
    }
    else if (std::find(CMDS_MOV2.begin(), CMDS_MOV2.end(), cmd.getValue()) != CMDS_MOV2.end())
    {
        return makeCmdNodeMov2(true);
    }
    else if (std::find(CMDS_MOVL.begin(), CMDS_MOVL.end(), cmd.getValue()) != CMDS_MOVL.end())
    {
        return makeCmdNodeMovL(true);
    }
    else if (std::find(CMDS_TRI.begin(), CMDS_TRI.end(), cmd.getValue()) != CMDS_TRI.end())
    {
        return makeCmdNodeTri(true);
    }
    else
    {
        std::cout << "[ASSEMBLER ERROR]: Unknow Token\n";
        next();
        return new NodeUni(Token(Token::ERROR, "Unknow Token", Position()));
    }

    /*
    for (unsigned int i = 0; i < CMDS_UNI.size(); i++)
    {
        if (CMDS_UNI[i].compare(cmd.getValue()) == 0)
        {
        }
    }
    for (unsigned int i = 0; i < CMDS_BIN.size(); i++)
    {
        if (CMDS_BIN[i].compare(cmd.getValue()) == 0)
        {
            return makeCmdNodeBin();
        }
    }
    for (unsigned int i = 0; i < CMDS_BINL.size(); i++)
    {
        if (CMDS_BINL[i].compare(cmd.getValue()) == 0)
        {
            return makeCmdNodeBinL();
        }
    }
    for (unsigned int i = 0; i < CMDS_MOV2.size(); i++)
    {
        if (CMDS_MOV2[i].compare(cmd.getValue()) == 0)
        {
            return makeCmdNodeMov2();
        }
    }
    for (unsigned int i = 0; i < CMDS_MOV.size(); i++)
    {
        if (CMDS_MOV[i].compare(cmd.getValue()) == 0)
        {
            return makeCmdNodeMov();
        }
    }
    for (unsigned int i = 0; i < CMDS_MOVL.size(); i++)
    {
        if (CMDS_MOVL[i].compare(cmd.getValue()) == 0)
        {
            return makeCmdNodeMovL();
        }
    }
    for (unsigned int i = 0; i < CMDS_TRI.size(); i++)
    {
        if (CMDS_TRI[i].compare(cmd.getValue()) == 0)
        {
            return makeCmdNodeTri();
        }
    }

    std::cout << "[ASSEMBLER ERROR]: Unknow Token\n";
    next();
    return new NodeUni(Token(Token::ERROR, "Missing argument", Position()));
    */
}

NodeBin *Parser::makeCmdNodeBin(bool print)
{
    if (print_debug)
        std::cout << index << "bin \n";
    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 2 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeBin(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token val1 = tokens[index];
    if (val1.getType() != Token::DEC && val1.getType() != Token::HEX && val1.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected hex, dec or reg at " << val1.getPos().getLine() << ":" << val1.getPos().getCol() << "\n";
        return new NodeBin(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 1 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeBin(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token val2 = tokens[index];
    if (val2.getType() != Token::DEC && val2.getType() != Token::HEX && val2.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg at " << val2.getPos().getLine() << ":" << val2.getPos().getCol() << "\n";
        return new NodeBin(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    return new NodeBin(cmd, val1, val2);
}

NodeMov *Parser::makeCmdNodeMov(bool print)
{
    if (print_debug)
        std::cout << index << "mov \n";
    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 2 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMov(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token reg = tokens[index];
    if (reg.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg at " << reg.getPos().getLine() << ":" << reg.getPos().getCol() << "\n";
        return new NodeMov(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 1 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMov(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token val = tokens[index];
    if (val.getType() != Token::DEC && val.getType() != Token::HEX && val.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg at " << val.getPos().getLine() << ":" << val.getPos().getCol() << "\n";
        return new NodeMov(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    return new NodeMov(cmd, reg, val);
}

NodeMov2 *Parser::makeCmdNodeMov2(bool print)
{
    if (print_debug)
        std::cout << index << "mov2 \n";
    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 3 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMov2(Token(Token::ERROR, "Missing argument", Position()), Token(), Token(), Token());
    }
    Token reg = tokens[index];
    if (reg.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg at " << reg.getPos().getLine() << ":" << reg.getPos().getCol() << "\n";
        return new NodeMov2(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 2 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMov2(Token(Token::ERROR, "Missing argument", Position()), Token(), Token(), Token());
    }
    Token val1 = tokens[index];
    if (val1.getType() != Token::DEC && val1.getType() != Token::HEX && val1.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg at " << val1.getPos().getLine() << ":" << val1.getPos().getCol() << "\n";
        return new NodeMov2(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 1 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMov2(Token(Token::ERROR, "Missing argument", Position()), Token(), Token(), Token());
    }
    Token val2 = tokens[index];
    if (val2.getType() != Token::DEC && val2.getType() != Token::HEX && val2.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 3 incorrect, expected hex, dec or reg at " << val2.getPos().getLine() << ":" << val2.getPos().getCol() << "\n";
        return new NodeMov2(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token(), Token());
    }
    next();
    return new NodeMov2(cmd, reg, val1, val2);
}

NodeBinL *Parser::makeCmdNodeBinL(bool print)
{
    if (print_debug)
        std::cout << index << "binL \n";
    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 2 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeBinL(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token val = tokens[index];
    if (val.getType() != Token::DEC && val.getType() != Token::HEX && val.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected hex, dec or reg at " << val.getPos().getLine() << ":" << val.getPos().getCol() << "\n";
        return new NodeBinL(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 1 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeBinL(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token label = tokens[index];
    if (label.getType() != Token::LABEL)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected label at " << label.getPos().getLine() << ":" << label.getPos().getCol() << "\n";
        return new NodeBinL(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    return new NodeBinL(cmd, val, label);
}

NodeMovL *Parser::makeCmdNodeMovL(bool print)
{
    if (print_debug)
        std::cout << index << "movL \n";
    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 2 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMovL(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token reg = tokens[index];
    if (reg.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected reg at " << reg.getPos().getLine() << ":" << reg.getPos().getCol() << "\n";
        return new NodeMovL(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 1 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeMovL(Token(Token::ERROR, "Missing argument", Position()), Token(), Token());
    }
    Token label = tokens[index];
    if (label.getType() != Token::LABEL)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected label at " << label.getPos().getLine() << ":" << label.getPos().getCol() << "\n";
        return new NodeMovL(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token());
    }
    next();
    return new NodeMovL(cmd, reg, label);
}

NodeTri *Parser::makeCmdNodeTri(bool print)
{
    if (print_debug)
        std::cout << index << "tri \n";
    Token cmd = tokens[index];
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 3 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeTri(Token(Token::ERROR, "Missing argument", Position()), Token(), Token(), Token());
    }
    Token val1 = tokens[index];
    if (val1.getType() != Token::DEC && val1.getType() != Token::HEX && val1.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 1 incorrect, expected hex, dec or reg at " << val1.getPos().getLine() << ":" << val1.getPos().getCol() << "\n";
        return new NodeTri(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 2 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeTri(Token(Token::ERROR, "Missing argument", Position()), Token(), Token(), Token());
    }
    Token val2 = tokens[index];
    if (val2.getType() != Token::DEC && val2.getType() != Token::HEX && val2.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 2 incorrect, expected hex, dec or reg at " << val2.getPos().getLine() << ":" << val2.getPos().getCol() << "\n";
        return new NodeTri(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token(), Token());
    }
    next();
    if (index == -1 || tokens[index].getType().compare(Token::CMD) == 0)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: Missing 1 argument at " << cmd.getPos().getLine() << ":" << cmd.getPos().getCol() << "\n";
        return new NodeTri(Token(Token::ERROR, "Missing argument", Position()), Token(), Token(), Token());
    }
    Token val3 = tokens[index];
    if (val3.getType() != Token::DEC && val3.getType() != Token::HEX && val3.getType() != Token::REG)
    {
        if (print)
            std::cout << "[ASSEMBLER ERROR]: type of argument 3 incorrect, expected hex, dec or reg at " << val3.getPos().getLine() << ":" << val3.getPos().getCol() << "\n";
        return new NodeTri(Token(Token::ERROR, "argument type incorrect", Position()), Token(), Token(), Token());
    }
    next();
    return new NodeTri(cmd, val1, val2, val3);
}