#include "assembler/Interpreter.hpp"
#include "assembler/Lexer.hpp"
#include "assembler/Parser.hpp"

#include "global.hpp"

#include "utils/console.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <codecvt>

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
                    std::stringstream debugStr;
                    debugStr << "[ASSEMBLER ERROR]: same label in file " << t.getPos().getFileName() << " at " << t.getPos().getLine() << ":" << t.getPos().getCol();
                    printError(debugStr.str());
                    return "FFFFFFFF ";
                }
                else
                {
                    labels.push_back(nodes[i]);
                    labels_adr.push_back(index);
                    printInfo("label " + nodes[i]->getToken(0).getValue() + ":" + std::to_string(index));
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
            else if (t.getType().compare(Token::LABEL_LOW) == 0 || t.getType().compare(Token::LABEL_HIGH) == 0)
            {
                index += 1;
            }
            else if (t.getType().compare(Token::STRING) == 0)
            {
                printDebug("String: " + std::to_string(t.getValue().size()+1));
                index += t.getValue().size()+1;
            }
            else if (t.getType().compare(Token::HEX) == 0 || t.getType().compare(Token::DEC) == 0 || t.getType().compare(Token::REG) == 0 || t.getType().compare(Token::BIN) == 0 || t.getType().compare(Token::CHAR) == 0)
            {
                index += 1;
            }
            else if (t.getType().compare(Token::CONSTANT) == 0)
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
                        std::stringstream debugStr;
                        debugStr << "[ASSEMBLER ERROR]: cannot import already imported file " << impName << " in " << t.getPos().getFileName() << " at " << t.getPos().getLine() << ":" << t.getPos().getCol();
                        printError(debugStr.str());
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
            else if (t.getType().compare(Token::ORIGIN) == 0)
            {
                index = strtol(t.getValue().c_str(), NULL, 16) & 0xFFFF;
            }
        }
        else if (nodes[i]->getType() == Node::NODE_DUO)
        {
            Token t = nodes[i]->getToken(0);
            if (t.getType().compare(Token::CONSTANT_DECLARE) == 0)
            {
                int valCode = getValCode(nodes[i]->getToken(1));
                constants.push_back(t.getValue());
                constants_val.push_back(valCode);
                printDebug("constant " + t.getValue() + "(" + std::to_string(valCode) + ")" + ":" + std::to_string(index));
            }
        }
        else if (nodes[i]->getType() == Node::NODE_MOV2L)
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
        printDebug(nodes[i]->toString());
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
    printDebug("find uni node");

    std::stringstream res;
    res << std::hex;
    Token t = n->getToken(0);
    if (t.getType().compare(Token::IMPORT) == 0 || t.getType().compare(Token::COMMENT) == 0 || t.getType().compare(Token::LABEL_DECLARE) == 0 || t.getType().compare(Token::ORIGIN) == 0)
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
    else if (t.getType().compare(Token::LABEL) == 0 || t.getType().compare(Token::LABEL_LOW) == 0 || t.getType().compare(Token::LABEL_HIGH) == 0)
    {
        std::string labelVal = getLabelVal(t);
        if (labelVal == "FFFFFFFF ")
        {
            return labelVal;
        }
        res << labelVal;
    }
    else if (t.getType().compare(Token::HEX) == 0 || t.getType().compare(Token::DEC) == 0 || t.getType().compare(Token::REG) == 0 || t.getType().compare(Token::BIN) == 0 || t.getType().compare(Token::CHAR) == 0 || t.getType().compare(Token::CONSTANT) == 0 )
    {
        int valCode = getValCode(t);
        res << std::uppercase << std::setfill('0') << std::setw(2) << valCode << " ";
    }
    else if (t.getType().compare(Token::STRING) == 0)
    {
        //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
       // std::wstring str = converter.from_bytes(t.getValue());
       std::string str = t.getValue();
        for (unsigned int i = 0; i < str.size(); i++)
        {
            uint8_t valStr = str[i];
            res << std::uppercase << std::setfill('0') << std::setw(2) << (uint32_t)valStr << " ";
        }
        res << std::uppercase << std::setfill('0') << std::setw(2) << 0 << " ";
    }
    else
    {
        res << std::uppercase << std::setfill('0') << std::setw(2) << -1 << " ";
    }
    return res.str();
}

std::string Interpreter::nodeBin(Node *n)
{
    printDebug("find bin node");
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
    printDebug("find mov node");
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
    printDebug("find mov2 node");
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
    printDebug("find binl node");
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
    std::string labelVal = getLabelVal(label);
    if (labelVal == "FFFFFFFF ")
    {
        return labelVal;
    }
    res << labelVal;

    return res.str();
}

std::string Interpreter::nodeMovL(Node *n)
{
    printDebug("find movl node");
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
    std::string labelVal = getLabelVal(label);
    if (labelVal == "FFFFFFFF ")
    {
        return labelVal;
    }
    res << labelVal;

    return res.str();
}

std::string Interpreter::nodeMov2L(Node *n)
{
    printDebug("find mov2l node");
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
        std::stringstream debugStr;
        debugStr << "[ASSEMBLER ERROR]: can't find label in file " << label.getPos().getFileName() << " at " << label.getPos().getLine() << ":" << label.getPos().getCol();
        printError(debugStr.str());
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
    printDebug("find tri node");
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
        std::stringstream debugStr;
        debugStr << "[ASSEMBLER ERROR] can't find reg code in file " << reg.getPos().getFileName() << " at " << reg.getPos().getLine() << ":" << reg.getPos().getCol();
        printError(debugStr.str());
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
    else if (val.getType().compare(Token::CHAR) == 0)
    {
        valCode = val.getValue()[0];
    }
    else if (val.getType().compare(Token::LABEL_LOW) == 0 || val.getType().compare(Token::LABEL_HIGH) == 0)
    {
        std::string labelVal = getLabelVal(val);
        if (labelVal == "FFFFFFFF ")
        {
            return -1;
        }
        //printDebug("\n*"+labelVal+"\n");
        valCode = stoi(labelVal, NULL, 16);
        //printDebug("\n**"+std::to_string(valCode)+"\n");
    }
    else if (val.getType().compare(Token::CONSTANT) == 0)
    {
        bool find = false;
        for (unsigned int i = 0; i < constants.size(); i++)
        {
            if (val.getValue().compare(constants[i]) == 0)
            {
                valCode = constants_val[i];
                find = true;
                break;
            }
        }
        if (!find)
        {
            std::stringstream debugStr;
            debugStr << "[ASSEMBLER ERROR] can't find constant code of " << val.getValue() << " in file " << val.getPos().getFileName() << " at " << val.getPos().getLine() << ":" << val.getPos().getCol();
            printError(debugStr.str());
            return -1;
        }
    }
    else
    {
        if (val.getType().compare(Token::HEX) == 0)
        {
            valCode = stoi(val.getValue(), NULL, 16);
        }
        else if (val.getType().compare(Token::BIN) == 0)
        {
            valCode = stoi(val.getValue(), NULL, 2);
        }
        else
        {
            valCode = stoi(val.getValue(), NULL, 10);
        }
    }
    return valCode;
}

std::string Interpreter::getLabelVal(Token lab)
{
    std::stringstream res;
    bool find = false;
    for (unsigned int i = 0; i < labels.size(); i++)
    {
        if (lab.getValue().compare(labels[i]->getToken(0).getValue()) == 0)
        {
            if (lab.getType().compare(Token::LABEL) == 0)
            {
                res << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] / 256) << " ";
                res << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] % 256) << " ";
            }
            else if (lab.getType().compare(Token::LABEL_LOW) == 0)
            {
                res << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] % 256) << " ";
            }
            else if (lab.getType().compare(Token::LABEL_HIGH) == 0)
            {
                res << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (labels_adr[i] / 256) << " ";
            }
            find = true;
            break;
        }
    }
    if (!find)
    {
        std::stringstream debugStr;
        debugStr << "[ASSEMBLER ERROR]: can't find label in file " << lab.getPos().getFileName() << " at " << lab.getPos().getLine() << ":" << lab.getPos().getCol();
        printError(debugStr.str());
        return "FFFFFFFF ";
    }
    return res.str();
}

std::vector<Node *> Interpreter::import(std::string fileName)
{
    importName.push_back(fileName);
    bool error = false;
    std::vector<Node *> nodesError = {new Node()};
    std::string debugStr = "";

    //lexing
    std::string file = "";
    try
    {
        file = readFile(fileName);
    }
    catch (std::string e)
    {
        printError(e);
        return nodes;
    }
    Lexer lexer(file, fileName);
    std::vector<Token> tokens = lexer.makeToken();

    //verif lexing
    debugStr.clear();
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].getType() == Token::ERR)
        {
            error = true;
        }
        if (print_debug)
        {
            debugStr += tokens[i].toString() + ", ";
        }
    }
    printDebug(debugStr + "\n");
    if (error)
    {
        printError("Error during Lexing in file " + fileName + ", cannot continue");
        return nodesError;
    }

    //parsing
    Parser parser(tokens);
    std::vector<Node *> parseNodes = parser.parse();

    //print parsing
    debugStr.clear();
    for (unsigned int i = 0; i < parseNodes.size(); i++)
    {
        if (print_debug)
            debugStr += nodes[i]->toString() + ", ";
        if (parseNodes[i]->getToken(0).getType().compare(Token::ERR) == 0)
        {
            error = true;
        }
    }
    printDebug(debugStr + "\n");
    if (error)
    {
        printError("Error during Parsing in file " + fileName + ", cannot continue");
        for (unsigned int i = 0; i < parseNodes.size(); i++)
        {
            delete parseNodes[i];
        }
        return nodesError;
    }
    return parseNodes;
}
