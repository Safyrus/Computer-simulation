#include "AssemblerCompiler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

AssemblerCompiler::AssemblerCompiler()
{
    char_comment = '#';
    char_label = ':';
    char_labelReplace = '£';
    char_reg = '$';
    char_decimal = '§';

    op_name.push_back("NOP");
    op_code.push_back(0);
    op_arg.push_back(0);
    op_label.push_back(0);
    op_size = 4;
}

AssemblerCompiler::AssemblerCompiler(const char *configFile)
{
}

AssemblerCompiler::~AssemblerCompiler()
{
}

void AssemblerCompiler::loadAssembler(const char *f)
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

            if(label && !comment)
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

    //print result
    std::cout << "#------------------------------#\n|Labels:\n";
    for (int i = 0; i < labels.size(); i++)
    {
        std::cout << labels_pos.at(i) << ": " << labels.at(i) << '\n';
    }
    std::cout << "#------------------------------#\n";

    std::cin.ignore();

    //open file
    file.open(f);
    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file\n";
        return;
    }
    else
    {
        std::cout << "file open 2\n";
    }

    lineCount = 0;

    //instruction and comment reading
    // for each line
    while (getline(file, line))
    {
        std::string word;

        int8_t cmd_arg = 0;
        int8_t cmd_argLabel = 0;
        bool comment = false;
        int8_t cmd_index = 0;
        bool label = false;
        //set label to true if this line contain label
        for (int i = 0; i < labels_pos.size(); i++)
        {
            if(labels_pos[i]==lineCount)
            {
                label = true;
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
            }
            //check if any arg remaind
            else if (cmd_arg < op_arg[cmd_index])
            {
                if (cmd_argLabel < op_label[cmd_index] && op_label_pos[cmd_index][cmd_argLabel] == cmd_arg)
                {
                    bool find = false;
                    while (false /* check label */)
                    {
                        if (false /* label exist */)
                        {
                            //replace label
                            find = true;
                            break;
                        }
                    }
                    if (!find)
                    {
                        //error: label does not exist
                    }
                    //add label
                    cmd_argLabel++;
                    cmd_arg++;
                }
                else
                {
                    if (false /* register char find */)
                    {
                        if (false /* convert error */)
                        {
                            //error: not a valid register name
                        }
                        else
                        {
                            //add reg to instruction
                        }
                    }
                    else if (false /* decimal char find */)
                    {
                        if (false /* convert error */)
                        {
                            //error: not a valid decimal number
                        }
                        else
                        {
                            //add dec to instruction
                        }
                    }
                    else
                    {
                        if (false /* convert error */)
                        {
                            //error: not a valid hexadecimal number
                        }
                        else
                        {
                            //add hex to instruction
                        }
                    }
                }
            }
            //command find (maybe)
            else
            {
                int i = 0;
                while (false /* command name not find */)
                {
                    if (false /* cmd name find */)
                    {
                        // add command
                        cmd_index = i;
                        break;
                    }
                }
                //error: not a command name
            }
        }
        std::cout << '\n';
        lineCount++;

        if (!comment && !label)
        {
            for (int i = op_arg[cmd_index]; i < op_size; i++)
            {
                //add 00 to instruction
            }
        }
    }

    //close file
    file.close();
    std::cout << "file close 2\n";

    //print result
    std::cout << "#------------------------------#\n|Labels:\n";
    for (int i = 0; i < labels.size(); i++)
    {
        std::cout << labels_pos.at(i) << ": " << labels.at(i) << '\n';
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
}