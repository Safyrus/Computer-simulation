#include "AssemblerCompiler.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

bool isHex(const std::string &s)
{
    char *p;
    strtol(s.c_str(), &p, 16);

    return (*p == 0);
}

AssemblerCompiler::AssemblerCompiler()
{
    char_comment = '#';
    char_label = ':';
    char_labelReplace = '*';
    char_reg = '$';
    char_decimal = '%';

    op_name = {
        "NOP", "RST", "OFF", "MOV", "MOV", "CMP", "CMP", "CMP", "CMP",
        "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR",
        "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR",
        "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR",
        "ADD", "ADC", "SUB", "SBB", "MUL", "DIV", "MOD", "AND", "OR", "XOR",
        "JMP", "JMP", "JMP", "JMP", "JMP",
        "GET", "GET", "GET", "GET", "GET",
        "SET", "SET", "SET", "SET", "SET", "SET", "SET", "SET", "SET", "SET"};
    op_code = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
        0x50, 0x51, 0x52, 0x53, 0x53,
        0x54, 0x55, 0x56, 0x57, 0x57,
        0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x5b, 0x5f};
    op_arg = {
        0, 0, 0, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 2,
        3, 3, 3, 3, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 2, 2};
    op_arg_type = {
        {}, {}, {}, {REG, REG}, {REG, VAL}, {REG, REG}, {VAL, REG}, {REG, VAL}, {VAL, VAL}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, REG, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, VAL, REG}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, REG, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, VAL, VAL}, {REG, REG, REG}, {REG, VAL, REG}, {REG, REG, VAL}, {REG, VAL, VAL}, {REG, LABEL}, {REG, REG, REG}, {REG, VAL, REG}, {REG, REG, VAL}, {REG, VAL, VAL}, {REG, LABEL}, {REG, REG, REG}, {REG, VAL, REG}, {REG, REG, VAL}, {REG, VAL, VAL}, {VAL, REG, REG}, {VAL, VAL, REG}, {VAL, REG, VAL}, {VAL, VAL, VAL}, {REG, LABEL}, {VAL, LABEL}};
    op_arg_size = 3;

    reg_name = {
        "A", "B", "C", "D", "E", "F"
    };
    reg_code = {
        0, 1, 2, 3, 4, 5
    };
}

AssemblerCompiler::AssemblerCompiler(const char *configFile)
{
}

AssemblerCompiler::~AssemblerCompiler()
{
}

void AssemblerCompiler::loadAssembler(const char *f)
{

    loadAssembler_read1(f);

    //print result
    std::cout << "#------------------------------#\n|Labels:\n";
    for (int i = 0; i < labels.size(); i++)
    {
        std::cout << labels_pos.at(i) << ": " << labels.at(i) << '\n';
    }
    std::cout << "#------------------------------#\n";

    std::cin.ignore();

    loadAssembler_read2(f);

    //print result
    std::cout << "#------------------------------#\n|Labels:\n";
    for (int i = 0; i < labels.size(); i++)
    {
        std::cout << labels_pos.at(i) << ": " << labels.at(i) << ", " << labels_val.at(i) << '\n';
    }
    std::cout << "|Instructions:\n";
    for (int i = 0; i < instructions.size(); i++)
    {
        std::cout << instructions_pos.at(i) << ": " << instructions.at(i) << '\n';
    }
    std::cout << "|Comments:\n";
    for (int i = 0; i < comments.size(); i++)
    {
        std::cout << comments_pos.at(i) << ": " << comments.at(i) << '\n';
    }
    std::cout << "#------------------------------#\n";
}

void AssemblerCompiler::loadBinary(const char *f)
{
}

void AssemblerCompiler::saveAssembler(const char *f)
{
}

void AssemblerCompiler::saveBinary(const char *f)
{
    //create file
    std::ofstream file(f);
    file << std::hex << std::setfill('0') << std::setw(2);
    std::cout << "CREATE FILE";
    //std::cin.ignore();

    //for each instruction
    for (int i = 0; i < instructions.size(); i++)
    {
        if(i%4==0 && i!=0)
        {
            file << '\n';
        }

        std::string word;
        std::string instructionsBuffer = instructions[i] + ' ';

        std::cout << "INSTRUCTION LINE: " << instructionsBuffer << '\n';
        //std::cin.ignore();

        word = instructionsBuffer.substr(0, instructionsBuffer.find(' '));
        instructionsBuffer = instructionsBuffer.substr(instructionsBuffer.find(' ') + 1);

        std::cout << "INSTRUCTION BUFFER: " << instructionsBuffer << '\n';
        std::cout << "WORD: " << word << '\n';
        //std::cin.ignore();

        int code = strtol(word.c_str(), NULL, 10);

        std::cout << "CODE: " << code << '\n';
        //std::cin.ignore();

        file << std::hex << std::setfill('0') << std::setw(2) << code << ' ';

        std::cout << "ADD INSTRUCTION TO FILE" << '\n';
        //std::cin.ignore();

        word = instructionsBuffer.substr(0, instructionsBuffer.find(' '));
        instructionsBuffer = instructionsBuffer.substr(instructionsBuffer.find(' ') + 1);
        int arg_size = 0;
        while (word != "" || arg_size < op_arg_size)
        {
            
            bool label = false;
            int label_index = 0;
            for (int j = 0; j < labels.size(); j++)
            {
                if(labels[j] == word)
                {
                    label = true;
                    label_index = j;
                    break;
                }
            }
            if(label)
            {
                std::cout << "LABEL ";
                file << std::hex << std::setfill('0') << std::setw(2) << (labels_val[label_index]>>8)%0xff << ' ' << labels_val[label_index]%0xff;
                arg_size++;
            }else if (word.front()==char_reg)
            {
                std::cout << "REG ";
                for (int j = 0; j < reg_name.size(); j++)
                {
                    if(reg_name[j]==word.substr(1))
                    {
                        file << std::hex << std::setfill('0') << std::setw(2) << std::to_string(reg_code[j]);
                        break;
                    }
                }
                
            }else if (word.front()==char_decimal)
            {
                std::cout << "DEC ";
                file << std::hex << std::setfill('0') << std::setw(2) << word.substr(1);
            }else if(word != "")
            {
                std::cout << "HEX ";
                file << std::hex << std::setfill('0') << std::setw(2) << word;
            }else
            {
                std::cout << "NOTHING ";
                file << std::hex << std::setfill('0') << std::setw(2) << "00";
            }
            
            std::cout << "WORD: " << word << "[" << arg_size << "]" << '\n';
            //std::cin.ignore();

            file << ' ';
            word = instructionsBuffer.substr(0, instructionsBuffer.find(' '));
            instructionsBuffer = instructionsBuffer.substr(instructionsBuffer.find(' ') + 1);
            arg_size++;
        }
        file << ' ';
        //std::cin.ignore();
    }

    //save file
    file.close();
}

void AssemblerCompiler::loadAssembler_read1(const char *f)
{
    //open file
    std::ifstream file(f);

    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file\n";
        return;
    }
    else
    {
        std::cout << "file open \n";
    }

    std::string line = "";
    int lineCount = 0;

    //-----label reading-----
    // for each line
    while (getline(file, line))
    {
        std::string word;
        bool comment = false;
        bool label = false;

        //for each word
        while (line != "")
        {
            //check if end of line
            word = line.substr(0, line.find(" "));
            if (word == line)
            {
                line = "";
            }
            else
            {
                line = line.substr(line.find(" ") + 1);
            }
            std::cout << word << " ";

            if (!comment && word.find(char_comment) != std::string::npos)
            {
                comment = true;
            }

            if (label && !comment)
            {
                std::cout << "**ERROR: SOMETHING AFTER LABEL**" << line;
                line = "";
            }

            //label symbol find a the end of the word
            if (word.back() == char_label && !comment)
            {
                std::cout << "**LABEL DETECTED**";
                word = word.substr(0, word.length() - 1);

                //special char like reg or dec find
                if (word.find(char_comment) != std::string::npos || word.find(char_reg) != std::string::npos || word.find(char_decimal) != std::string::npos || word.find(char_labelReplace) != std::string::npos || word.find(char_label) != std::string::npos)
                {
                    std::cout << "**ERROR: LABEL NAME INCORRECT**";
                }
                else
                {
                    //add label
                    std::cout << "**ADD LABEL**";
                    labels.push_back(word);
                    labels_pos.push_back(lineCount);
                    labels_val.push_back(0);
                    std::cout << labels.back();
                    label = true;
                }
            }
        }
        std::cout << '\n';
        lineCount++;
    }

    //close file
    file.close();
    std::cout << "file close\n";
}

void AssemblerCompiler::loadAssembler_read2(const char *f)
{
    //open file
    std::ifstream file(f);

    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file\n";
        return;
    }
    else
    {
        std::cout << "file open 2\n";
    }

    std::string line = "";
    int lineCount = 0;
    int instructionCounter = 0;

    //instruction and comment reading
    // for each line
    while (getline(file, line))
    {
        std::string word;

        int cmd_arg = 0;
        std::string cmd = "";
        bool comment = false;
        bool label = false;
        std::vector<int> cmd_find;

        //set label to true if this line contain label
        for (int i = 0; i < labels_pos.size(); i++)
        {
            if (labels_pos[i] == lineCount)
            {
                label = true;
                labels_val[i] = instructionCounter * (op_arg_size+1);
                break;
            }
        }

        //for each word
        while (line != "")
        {
            //check if end of line
            word = line.substr(0, line.find(" "));
            if (word == line)
            {
                line = "";
            }
            else
            {
                line = line.substr(line.find(" ") + 1);
            }
            std::cout << word << " ";

            //comment symbol find
            if (word.find(char_comment) != std::string::npos)
            {
                std::cout << "**COMMENT DETECTED**";
                int char_com_pos = word.find(char_comment);
                std::string strBefor = word.substr(0, (char_com_pos < 0) ? 0 : char_com_pos);
                std::string strAfter = word.substr(word.find(char_comment) + 1);
                if (strBefor != "")
                {
                    std::cout << "**SOMETHING BEFOR COMMENT**\n";
                    line = strBefor + ' ' + char_comment + strAfter + " " + line;
                }
                else
                {
                    comment = true;
                    std::cout << "**ADD COMMENT**" << strAfter + " " + line;
                    comments.push_back(strAfter + " " + line);
                    comments_pos.push_back(lineCount);
                    line = "";
                }
            }
            //check if label is on the line
            else if (label)
            {
                std::cout << "**LABEL FIND**";
                label = false;
            }
            //check if any arg remaind
            else if (!cmd_find.empty())
            {
                std::cout << "**ARGUMENT FOUND**";

                //check what type of argument is possible
                bool reg = false;
                bool val = false;
                bool lab = false;
                int argSize = 0;
                for (int i = 0; i < cmd_find.size(); i++)
                {
                    if (op_arg_type[cmd_find[i]][cmd_arg] == REG)
                    {
                        reg = true;
                    }
                    else if (op_arg_type[cmd_find[i]][cmd_arg] == LABEL)
                    {
                        lab = true;
                    }
                    else if (op_arg_type[cmd_find[i]][cmd_arg] == VAL)
                    {
                        val = true;
                    }
                    if (argSize < op_arg[cmd_find[i]])
                    {
                        argSize = op_arg[cmd_find[i]];
                    }
                }
                if (argSize <= cmd_arg)
                {
                    std::cout << "**ERROR: TOO MANY ARG**";
                }
                else
                {
                    int change = -1;
                    if (word.front() == char_reg && reg)
                    {
                        std::cout << "**TYPE REG**";
                        cmd += word + ' ';
                        change = REG;
                        cmd_arg++;
                    }
                    else if ((word.front() == char_decimal || isHex(word)) && val)
                    {
                        std::cout << "**TYPE VAL**";
                        cmd += word + ' ';
                        change = VAL;
                        cmd_arg++;
                    }
                    else
                    {
                        bool findLabel = false;
                        for (int i = 0; i < labels.size(); i++)
                        {
                            if (labels.at(i) == word)
                            {
                                findLabel = true;
                                break;
                            }
                        }
                        if (findLabel)
                        {
                            std::cout << "**TYPE LABEL**";
                            cmd += word + ' ';
                            change = LABEL;
                            cmd_arg++;
                        }
                        else
                        {
                            std::cout << "**ERROR: ARG TYPE INCORRECT**";
                        }
                    }

                    //change possible command
                    if (change > -1)
                    {
                        std::cout << "**CHANGE**";
                        std::vector<int> tmp;
                        for (int i = 0; i < cmd_find.size(); i++)
                        {
                            if (op_arg_type[cmd_find[i]][cmd_arg - 1] == change)
                            {
                                tmp.push_back(cmd_find.at(i));
                            }
                        }
                        cmd_find = tmp;
                    }

                    std::cout << "*[" << cmd_find.size() << "][" << cmd_arg << "][" << (int)(op_arg[cmd_find[0]]) << "]*";
                    if (cmd_find.size() == 1 && cmd_arg == op_arg[cmd_find[0]])
                    {
                        std::cout << "**ADD CMD**";
                        cmd = std::to_string(op_code[cmd_find[0]]) + cmd.substr(cmd.find(' '));
                        instructions.push_back(cmd);
                        instructions_pos.push_back(lineCount);
                    }
                }
            }
            //command find (maybe)
            else
            {
                std::cout << "**CHECK COMMAND NAME**";
                int i = 0;
                bool addCmd = false;
                // while command name not find
                while (i < op_name.size())
                {
                    //cmd name find
                    if (op_name[i] == word)
                    {
                        cmd_find.push_back(i);
                        // add command
                        if (!addCmd)
                        {
                            cmd += word + ' ';
                            addCmd = true;
                        }
                    }
                    i++;
                }
                if (cmd_find.empty())
                {
                    std::cout << "**ERROR: NOT A COMMAND NAME**";
                }
                else
                {
                    std::cout << "**COMMAND NAME FOUND[" << cmd_find.size() << "]**";
                    instructionCounter++;
                    if (cmd_find.size() == 1 && op_arg[cmd_find.at(0)] == 0)
                    {
                        std::cout << "**ADD CMD**";
                        instructions.push_back(std::to_string(op_code[cmd_find[0]]));
                        instructions_pos.push_back(lineCount);
                    }
                }
            }
        }
        std::cout << '\n';
        lineCount++;
    }

    //close file
    file.close();
    std::cout << "file close 2\n";
}