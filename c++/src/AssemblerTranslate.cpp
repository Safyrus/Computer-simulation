#include "AssemblerTranslate.hpp"

AssemblerTranslate::AssemblerTranslate()
{
}

AssemblerTranslate::AssemblerTranslate(const char *configFile)
{
    
}

AssemblerTranslate::~AssemblerTranslate()
{
}

void AssemblerTranslate::loadAssembler(const char* file)
{
    //open file
    if(/* can't open */)
    {
        //error: can't open file
    }

    //label reading
    while (/* line */)
    {
        bool find = false;

        while (/* word and not find */)
        {
            if(/* label symbol find a the end of the word */)
            {
                if(/* special char like reg or dec find */)
                {
                    //error: label name incorrect
                }
                find = true;
                //add label
            }
        }
    }
    
    //instruction and comment reading
    while (/* line */)
    {
        int8_t cmd_arg = 0;
        int8_t cmd_argLabel = 0;
        bool comment = false;
        bool label = false;
        int8_t cmd_index = 0;

        while (/* word */)
        {
            if(comment)
            {
                // add comment
            }
            else if(/* comment symbol find */)
            {
                comment = true;
                // add comment if there is something just after the symbol
            }
            else if (label)
            {
                // error: something wrong after label
            }
            else if (/* label symobl find */)
            {
                label = true;
                // ignore
            }
            else if (cmd_arg<op_arg[cmd_index])
            {
                if(cmd_argLabel<op_label[cmd_index] && op_label_pos[cmd_index][cmd_argLabel]==cmd_arg)
                {
                    bool find = false;
                    while (/* check label */)
                    {
                        if(/* label exist */)
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
                    if (/* register char find */)
                    {
                        if(/* convert error */)
                        {
                            //error: not a valid register name
                        }
                        else
                        {
                            //add reg to instruction
                        }
                    }
                    else if (/* decimal char find */)
                    {
                        if(/* convert error */)
                        {
                            //error: not a valid decimal number
                        }
                        else
                        {
                            //add dec to instruction
                        }
                    }else
                    {
                        if(/* convert error */)
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
            else if (/* command find */)
            {
                int i=0;
                while (/* command name not find */)
                {
                    if(/* cmd name find */)
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
            for (int i = op_arg[cmd_index]; i < op_size; i++)
            {
                //add 00 to instruction
            }
        }
    }

    //close file
}

void AssemblerTranslate::loadBinary(const char *file)
{
}

void AssemblerTranslate::saveAssembler(const char *file)
{
}

void AssemblerTranslate::saveBinary(const char *file)
{
}