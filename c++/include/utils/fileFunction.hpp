#ifndef FILEFUNCTION_HPP
#define FILEFUNCTION_HPP

#include <string>
#include <fstream>
#include <iostream>

#include "global.hpp"

std::string openFile(std::string f)
{
    //open file
    std::string fileName = f;
    std::ifstream file;
    file.open(f);
    if (!file.is_open())
    {
        std::string e = "Unable to open file " + fileName + "\n";
        throw e;
    }
    else
    {
        if (print_debug)
            std::cout << "file " + fileName + " open \n";
    }

    // get file content
    if (print_debug)
        std::cout << "read file " + fileName + " ...";
    std::string str = "";
    std::string line = "";
    while (getline(file, line))
    {
        str += line + '\n';
    }
    if (print_debug)
        std::cout << "done\n";

    //close file
    file.close();
    if (print_debug)
        std::cout << "file " + fileName + " close\n";

    return str;
}

void writeFile(std::string content, std::string fileName)
{
    std::ofstream file;
    file.open(fileName);
    int spaceCount = 0;
    for (unsigned int i = 0; i < content.size(); i++)
    {
        file << content[i];
        if (content[i] == ' ')
        {
            spaceCount++;
            if (spaceCount >= 16)
            {
                file << '\n';
                spaceCount = 0;
            }
            else if (spaceCount % 4 == 0)
            {
                file << ' ';
            }
        }
    }
    file.close();
}

#endif // FILEFUNCTION_HPP