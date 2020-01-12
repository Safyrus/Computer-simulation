#include "AssemblerCompiler.hpp"

AssemblerCompiler::AssemblerCompiler()
{
}

AssemblerCompiler::AssemblerCompiler(const char *configFile)
{
}

AssemblerCompiler::~AssemblerCompiler()
{
}

void AssemblerCompiler::loadAssembler(const char* file)
{
    //open file
    if(false/* can't open */)
    {
        //error: can't open file
    }

    //label reading
    while (false/* line */)
    {
        bool find = false;

        while (false/* word and not find */)
        {
            if(false/* label symbol find a the end of the word */)
            {
                if(false/* special char like reg or dec find */)
                {
                    //error: label name incorrect
                }
                find = true;
                //add label
            }
        }
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
}

void AssemblerCompiler::loadBinary(const char *file)
{
}

void AssemblerCompiler::saveAssembler(const char *file)
{
}

void AssemblerCompiler::saveBinary(const char *file)
{
}