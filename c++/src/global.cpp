#include <iostream>
#include <fstream>

#include "global.hpp"

#include "utils/console.hpp"

bool print_debug = false;
sf::Font baseFont;

std::string readFile(std::string f)
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
        printDebug("file " + fileName + " open");
    }

    // get file content
    printDebug("read file " + fileName + " ...");
    std::string str = "";
    std::string line = "";
    while (getline(file, line))
    {
        str += line + '\n';
    }
    printDebug("done");

    //close file
    file.close();
    printDebug("file " + fileName + " close");

    return str;
}

void writeHexFile(std::string content, std::string fileName)
{
    // open file
    std::ofstream file;
    file.open(fileName);

    // write strings of hex numbers in the file
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

    // close file
    file.close();
}
