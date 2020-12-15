#include "assembler/Interpreter.hpp"
#include "assembler/Lexer.hpp"
#include "assembler/Parser.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "global.hpp"

std::vector<std::string> Interpreter::importName;

const std::vector<std::string> Interpreter::REG_NAME = {"O", "A", "B", "C", "D", "E", "F", "R", "J1", "J2", "G0", "G1", "G2", "G3", "G4", "G5"};
const std::vector<int> Interpreter::REG_CODE = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const std::vector<std::string> Interpreter::CMDS_NAME = {"NOP", "RST", "OFF", "MOV", "CMP", "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR", "JMP", "GET", "SET"};
const std::vector<std::vector<int>> Interpreter::CMDS_CODE = {
    {0x0},
    {0x1},
    {0x2},
    {0x3, 0x4},
    {0x5, 0x6, 0x7, 0x8},
    {0x10, 0x20, 0x30, 0x40},
    {0x11, 0x21, 0x31, 0x41},
    {0x12, 0x22, 0x32, 0x42},
    {0x13, 0x23, 0x33, 0x43},
    {0x14, 0x24, 0x34, 0x44},
    {0x15, 0x25, 0x35, 0x45},
    {0x16, 0x26, 0x36, 0x46},
    {0x17, 0x27, 0x37, 0x47},
    {0x18, 0x28, 0x38, 0x48},
    {0x19, 0x29, 0x39, 0x49},
    {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57},
    {0x60, 0x61, 0x62, 0x63},
    {0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B}};

Interpreter::Interpreter(std::vector<Node *> nodes, std::string fileName)
{
    this->nodes = nodes;
    index = 0;
    importName.push_back(fileName);
}

Interpreter::~Interpreter()
{
}

std::string Interpreter::interprete()
{
    index = 0;
    std::string res = "";

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->getType() == Node::NODE_UNI)
        {
            Token t = nodes[i]->getToken(0);
            if (t.getType().compare(Token::LABEL_DECLARE) == 0)
            {
                bool sameLabel = false;
                for (unsigned int i = 0; i < labels.size(); i++)
                {
                    if (t.getValue().compare(labels[i]->getToken(0).getValue()) == 0)
                    {
                        sameLabel = true;
                        break;
                    }
                }
                if (sameLabel)
                {
                    std::cout << "[ASSEMBLER ERROR]: same label in file " << t.getPos().getFileName() << " at " << t.getPos().getLine() << ":" << t.getPos().getCol() << "\n";
                }
                else
                {
                    labels.push_back(nodes[i]);
                    labels_adr.push_back(index);
                    if(print_debug)
                        std::cout << "label " << nodes[i]->getToken(0).getValue() << ":" << index << "\n";
                }
            }
            else if (t.getType().compare(Token::CMD) == 0)
            {
                index += 4;
            }
            else if (t.getType().compare(Token::LABEL) == 0)
            {
                index += 2;
            }
            else if (t.getType().compare(Token::HEX) == 0 || t.getType().compare(Token::DEC) == 0 || t.getType().compare(Token::REG) == 0)
            {
                index += 1;
            }
            else if (t.getType().compare(Token::IMPORT) == 0)
            {
                std::string impName = t.getValue();
                for (unsigned int i = 0; i < importName.size(); i++)
                {
                    if (importName[i].compare(impName) == 0)
                    {
                        std::cout << "[ASSEMBLER ERROR]: cannot import already imported file " << impName << " in " << t.getPos().getFileName() << " at " << t.getPos().getLine() << ":" << t.getPos().getCol() << "\n";
                        return "FFFFFFFF ";
                    }
                }
                std::vector<Node *> importNodes = import(impName);
                for (unsigned int i = 0; i < importNodes.size(); i++)
                {
                    if (importNodes[i]->getType() == Node::NODE)
                    {
                        for (unsigned int i = 0; i < importNodes.size(); i++)
                        {
                            delete importNodes[i];
                        }
                        return "FFFFFFFF ";
                    }
                }
                nodes.insert(nodes.begin() + i + 1, importNodes.begin(), importNodes.end());
            }
        }
        else if(nodes[i]->getType() == Node::NODE_MOV2L)
        {
            index += 8;
        }
        else
        {
            index += 4;
        }
    }

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (print_debug)
            nodes[i]->print();
        if (nodes[i]->getType() == Node::NODE_UNI)
        {
            res += nodeUni(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_BIN)
        {
            res += nodeBin(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_BINL)
        {
            res += nodeBinL(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_MOV)
        {
            res += nodeMov(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_MOV2)
        {
            res += nodeMov2(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_MOVL)
        {
            res += nodeMovL(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_MOV2L)
        {
            res += nodeMov2L(nodes[i]);
        }
        else if (nodes[i]->getType() == Node::NODE_TRI)
        {
            res += nodeTri(nodes[i]);
        }
    }

    return res;
}

std::string Interpreter::nodeUni(Node *n)
{
    if (print_debug)
        std::cout << "find uni node\n";
    std::stringstream res;
    res << std::hex;
    Token t = n->getToken(0);
    if (t.getType().compare(Token::IMPORT) == 0)
    { /*
        std::string impName = t.getValue();
        for (unsigned int i = 0; i < importName.size(); i++)
        {
            if (importName[i].compare(impName) == 0)
            {
                std::cout << "[ASSEMBLER ERROR]: already import\n";
                return "FFFFFFFF ";
            }
        }
        res << import(impName);*/
    }
    else if (t.getType().compare(Token::COMMENT) == 0 || t.getType().compare(Token::LABEL_DECLARE) == 0)
    {
    }
    else if (t.getType().compare(Token::CMD) == 0)
    {
        int cmdCode = -1;
        for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
        {
            if (CMDS_NAME[i].compare(t.getValue()) == 0)
            {
                cmdCode = i;
                break;
            }
        }
        res << std::uppercase << std::setfill('0') << std::setw(2) << CMDS_CODE[cmdCode][0] << " 00 00 00 ";
    }
    else if (t.getType().compare(Token::LABEL) == 0)
    {
        bool find = false;
        for (unsigned int i = 0; i < labels.size(); i++)
        {
            if (t.getValue().compare(labels[i]->getToken(0).getValue()) == 0)
            {
                res << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] / 256) << " ";
                res << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] % 256) << " ";
                find = true;
                break;
            }
        }
        if (!find)
        {
            std::cout << "[ASSEMBLER ERROR]: can't find label in file " << t.getPos().getFileName() << " at " << t.getPos().getLine() << ":" << t.getPos().getCol() << "\n";
            return "FFFFFFFF ";
        }
    }
    else if (t.getType().compare(Token::HEX) == 0 || t.getType().compare(Token::DEC) == 0 || t.getType().compare(Token::REG) == 0)
    {
        int valCode = getValCode(t);
        res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";
    }
    else
    {
        res << std::uppercase << std::setfill('0') << std::setw(2) << -1 << " ";
    }
    return res.str();
}

std::string Interpreter::nodeBin(Node *n)
{
    if (print_debug)
        std::cout << "find bin node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token val1 = n->getToken(1);
    Token val2 = n->getToken(2);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    int cmdCodeType = 0;
    cmdCodeType += (val1.getType().compare(Token::REG) == 0) ? 0 : 1;
    cmdCodeType += (val2.getType().compare(Token::REG) == 0) ? 0 : 2;
    cmdCode = CMDS_CODE[cmdCode][cmdCodeType];
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " 00 ";

    //find vals code
    int valCode = getValCode(val1);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";
    valCode = getValCode(val2);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";

    return res.str();
}

std::string Interpreter::nodeMov(Node *n)
{
    if (print_debug)
        std::cout << "find mov node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token reg = n->getToken(1);
    Token val = n->getToken(2);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    int cmdCodeType = 0;
    cmdCodeType += (val.getType().compare(Token::REG) == 0) ? 0 : 1;
    cmdCode = CMDS_CODE[cmdCode][cmdCodeType];
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";

    //find reg code
    int regCode = getValCode(reg);
    res << std::uppercase << std::setfill('0') << std::setw(2) << regCode << " ";

    //find val code
    int valCode = getValCode(val);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";

    return res.str() + "00 ";
}

std::string Interpreter::nodeMov2(Node *n)
{
    if (print_debug)
        std::cout << "find mov2 node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token reg = n->getToken(1);
    Token val1 = n->getToken(2);
    Token val2 = n->getToken(3);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    int cmdCodeType = 0;
    cmdCodeType += (val1.getType().compare(Token::REG) == 0) ? 0 : 1;
    cmdCodeType += (val2.getType().compare(Token::REG) == 0) ? 0 : 2;
    cmdCode = CMDS_CODE[cmdCode][cmdCodeType];
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";

    //find reg code
    int regCode = getRegCode(reg);
    if (regCode == -1)
    {
        return "";
    }
    res << std::uppercase << std::setfill('0') << std::setw(2) << regCode << " ";

    //find val code
    int valCode = getValCode(val1);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";
    valCode = getValCode(val2);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";

    return res.str();
}

std::string Interpreter::nodeBinL(Node *n)
{
    if (print_debug)
        std::cout << "find binl node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token val = n->getToken(1);
    Token label = n->getToken(2);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    cmdCode = CMDS_CODE[cmdCode][(val.getType().compare(Token::REG) == 0) ? 3 : 7];
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";

    //find val code
    int valCode = getValCode(val);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";

    //find label code
    bool find = false;
    for (unsigned int i = 0; i < labels.size(); i++)
    {
        if (label.getValue().compare(labels[i]->getToken(0).getValue()) == 0)
        {
            res << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] / 256) << " ";
            res << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] % 256) << " ";
            find = true;
            break;
        }
    }
    if (!find)
    {
        std::cout << "[ASSEMBLER ERROR]: can't find label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol() << "\n";
        return "FFFFFFFF ";
    }

    return res.str();
}

std::string Interpreter::nodeMovL(Node *n)
{
    if (print_debug)
        std::cout << "find movl node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token reg = n->getToken(1);
    Token label = n->getToken(2);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    cmdCode = CMDS_CODE[cmdCode][3];
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";

    //find reg code
    int regCode = getRegCode(reg);
    if (regCode == -1)
    {
        return "";
    }
    res << std::uppercase << std::setfill('0') << std::setw(2) << regCode << " ";

    //find label code
    bool find = false;
    for (unsigned int i = 0; i < labels.size(); i++)
    {
        if (label.getValue().compare(labels[i]->getToken(0).getValue()) == 0)
        {
            res << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] / 256) << " ";
            res << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] % 256) << " ";
            find = true;
            break;
        }
    }
    if (!find)
    {
        std::cout << "[ASSEMBLER ERROR]: can't find label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol() << "\n";
        return "FFFFFFFF ";
    }

    return res.str();
}

std::string Interpreter::nodeMov2L(Node *n)
{
    if (print_debug)
        std::cout << "find mov2l node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token reg1 = n->getToken(1);
    Token reg2 = n->getToken(2);
    Token label = n->getToken(3);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    cmdCode = CMDS_CODE[cmdCode][1];

    //find reg code
    int regCode1 = getRegCode(reg1);
    if (regCode1 == -1)
    {
        return "FFFFFFFF ";
    }
    int regCode2 = getRegCode(reg2);
    if (regCode2 == -1)
    {
        return "FFFFFFFF ";
    }

    //find label code
    bool find = false;
    int label_adr = -1;
    for (unsigned int i = 0; i < labels.size(); i++)
    {
        if (label.getValue().compare(labels[i]->getToken(0).getValue()) == 0)
        {
            label_adr = labels_adr[i];
            find = true;
            break;
        }
    }
    if (!find)
    {
        std::cout << "[ASSEMBLER ERROR]: can't find label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol() << "\n";
        return "FFFFFFFF ";
    }

    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";
    res << std::uppercase << std::setfill('0') << std::setw(2) << regCode1 << " ";
    res << std::uppercase << std::setfill('0') << std::setw(2) << (label_adr / 256) << " 00 ";
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";
    res << std::uppercase << std::setfill('0') << std::setw(2) << regCode2 << " ";
    res << std::uppercase << std::setfill('0') << std::setw(2) << (label_adr % 256) << " 00 ";

    return res.str();
}

std::string Interpreter::nodeTri(Node *n)
{
    if (print_debug)
        std::cout << "find tri node\n";
    std::stringstream res;
    res << std::hex;
    Token cmd = n->getToken(0);
    Token val1 = n->getToken(1);
    Token val2 = n->getToken(2);
    Token val3 = n->getToken(3);

    //find cmd code
    int cmdCode = -1;
    for (unsigned int i = 0; i < CMDS_NAME.size(); i++)
    {
        if (CMDS_NAME[i].compare(cmd.getValue()) == 0)
        {
            cmdCode = i;
            break;
        }
    }
    int cmdCodeType = 0;
    cmdCodeType += (val1.getType().compare(Token::REG) == 0) ? 0 : 4;
    cmdCodeType += (val2.getType().compare(Token::REG) == 0) ? 0 : 1;
    cmdCodeType += (val3.getType().compare(Token::REG) == 0) ? 0 : 2;

    cmdCode = CMDS_CODE[cmdCode][cmdCodeType];
    res << std::uppercase << std::setfill('0') << std::setw(2) << cmdCode << " ";

    //find vals code
    int valCode = getValCode(val1);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";
    valCode = getValCode(val2);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";
    valCode = getValCode(val3);
    res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";

    return res.str();
}

int Interpreter::getRegCode(Token reg)
{
    int index = -1;
    for (unsigned int i = 0; i < REG_NAME.size(); i++)
    {
        if (reg.getValue().compare(REG_NAME[i]) == 0)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        return REG_CODE[index];
    }
    else
    {
        std::cout << "[ASSEMBLER ERROR] can't find reg code in file " << reg.getPos().getFileName() << " at " << reg.getPos().getLine() << ":" << reg.getPos().getCol() << "\n";
        return -1;
    }
}

int Interpreter::getValCode(Token val)
{
    int valCode = -1;
    if (val.getType().compare(Token::REG) == 0)
    {
        valCode = getRegCode(val);
        if (valCode == -1)
        {
            return -1;
        }
    }
    else
    {
        if (val.getType().compare(Token::HEX) == 0)
        {
            valCode = stoi(val.getValue(), NULL, 16);
        }
        else
        {
            valCode = stoi(val.getValue(), NULL, 10);
        }
    }
    return valCode;
}

std::vector<Node *> Interpreter::import(std::string fileName)
{
    importName.push_back(fileName);
    bool error = false;
    std::vector<Node *> nodesError = {new Node()};

    //lexing
    std::string file = "";
    try
    {
        file = openFile(fileName);
    }
    catch (std::string e)
    {
        std::cout << e;
        return nodes;
    }
    Lexer lexer(file, fileName);
    std::vector<Token> tokens = lexer.makeToken();

    //verif lexing
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == Token::ERR)
        {
            error = true;
        }
        if (print_debug)
        {
            tokens[i].print();
            std::cout << ", ";
        }
    }
    if (print_debug)
        std::cout << "\n\n";
    if (error)
    {
        std::cout << "Error during Lexing in file " + fileName + ", cannot continue\n"
                  << std::flush;
        return nodesError;
    }

    //parsing
    Parser parser(tokens);
    std::vector<Node *> parseNodes = parser.parse();

    //print parsing
    for (unsigned int i = 0; i < parseNodes.size(); i++)
    {
        if (print_debug)
            parseNodes[i]->print();
        if (parseNodes[i]->getToken(0).getType().compare(Token::ERR) == 0)
        {
            error = true;
        }
        if (print_debug)
            std::cout << ", " << std::flush;
    }
    if (print_debug)
        std::cout << "\n\n";
    if (error)
    {
        std::cout << "Error during Parsing in file " + fileName + ", cannot continue\n"
                  << std::flush;
        for (unsigned int i = 0; i < parseNodes.size(); i++)
        {
            delete parseNodes[i];
        }
        return nodesError;
    }
    return parseNodes;

    //interpreting
    /*
    Interpreter interpreter(nodes, fileName);
    fileOut = interpreter.interprete();

    //verif interpreting
    if (print_debug)
    {
        std::cout << "\n\n";
        std::cout << fileOut;
    }

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        delete nodes[i];
    }

    return fileOut;*/
}