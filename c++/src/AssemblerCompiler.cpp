#include "AssemblerCompiler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

AssemblerCompiler::AssemblerCompiler()
{
    char_comment = '#';
    char_label = ':';
    char_labelReplace = '£';
    char_reg = '$';
    char_decimal = '§';
}

AssemblerCompiler::AssemblerCompiler(const char *configFile)
{
}

AssemblerCompiler::~AssemblerCompiler()
{
}

void AssemblerCompiler::loadAssembler(const char* f)
{
    //open file
    //std::string line;
    std::ifstream file (f);

    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file\n";
        return;
    }
    std::cout << "file open\n";

    //-----label reading-----
    // for each line
    std::string line="";
    while (getline(file,line))
    {
        bool find = false;
        std::string word;

        //for each word
        while (line != "" && !find)
        {
            word = line.substr(0, line.find(" "));
            if(word==line)
            {
                line = "";
            }
            std::cout << word << " ";
            line = line.substr(line.find(" ")+1);

            if(word.back()== char_label)//label symbol find a the end of the word
            {
                /*std::cout << "**LABEL DETECTED**";
                if(word.find(char_comment)==0 || word.find(char_reg)==0 || word.find(char_decimal)==0 || word.find(char_labelReplace)==0)//special char like reg or dec find
                {
                    std::cout << "**ERROR: LABEL NAME INCORRECT**";
                }*/
                //find = true;
                //add label
            }
        }
        std::cout << '\n';
        

        // for each word
        //while (word != NULL && !find)
        //{
            //if(false/* label symbol find a the end of the word */)
            //{
                //if(false/* special char like reg or dec find */)
                //{
                    //error: label name incorrect
                //}
                //find = true;
                //add label
            //}
            //std::cout << word << " ";
            //word = strtok(NULL, " ");
        //}
    }
    
    //instruction and comment reading
    while (false/* line */)
    {
        int8_t cmd_arg = 0;
        int8_t cmd_argLabel = 0;
        bool comment = false;
        bool label = false;
        int8_t cmd_index = 0;

        while (false/* word */)
        {
            if(comment)
            {
                // add comment
            }
            else if(false/* comment symbol find */)
            {
                comment = true;
                // add comment if there is something just after the symbol
            }
            else if (label)
            {
                // error: something wrong after label
            }
            else if (false/* label symobl find */)
            {
                label = true;
                // ignore
            }
            else if (cmd_arg<op_arg[cmd_index])
            {
                if(cmd_argLabel<op_label[cmd_index] && op_label_pos[cmd_index][cmd_argLabel]==cmd_arg)
                {
                    bool find = false;
                    while (false/* check label */)
                    {
                        if(false/* label exist */)
                        {
                            //replace label
                            find = true;
                            break;
                        }
                    }
                    if(!find)
                    {
                        //error: label does not exist
                    }
                    //add label                    
                    cmd_argLabel++;
                    cmd_arg++;
                }
                else
                {
                    if (false/* register char find */)
                    {
                        if(false/* convert error */)
                        {
                            //error: not a valid register name
                        }
                        else
                        {
                            //add reg to instruction
                        }
                    }
                    else if (false/* decimal char find */)
                    {
                        if(false/* convert error */)
                        {
                            //error: not a valid decimal number
                        }
                        else
                        {
                            //add dec to instruction
                        }
                    }else
                    {
                        if(false/* convert error */)
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
            else if (false/* command find */)
            {
                int i=0;
                while (false/* command name not find */)
                {
                    if(false/* cmd name find */)
                    {
                        // add command
                        cmd_index = i;
                        break;
                    }
                }
                //error: not a command name
            }
            else
            {
                //error: something is wrong
            }            
        }
        if(!comment && !label)
        {
            for (int i = op_arg[cmd_index]; i < *op_size; i++)
            {
                //add 00 to instruction
            }
        }
    }

    //close file
    file.close();
    std::cout << "file close\n";
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