#ifndef HEXTXTTOBIN_HPP
#define HEXTXTTOBIN_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

std::vector<unsigned char> hexTxtToBin(std::string textFilePath)
{
    std::vector<unsigned char> binVect;

    std::ifstream file;
    file.open(textFilePath);
    if(!file.is_open())
    {
        std::cout << "/!\\ ERROR: hexfile conversion, open file fail" << std::endl;
        return binVect;
    }

    std::string line;
    std::string word;
    while (std::getline(file, word, ' '))
    {
        if(word.find('\n') != std::string::npos)
        {
            std::string s1 = word.substr(0, word.find('\n'));
            std::string s2 = word.substr(word.find('\n')+1);
            std::cout << s1 << "|" << s2 << "|";
            binVect.push_back(std::stoi(s1, nullptr, 16));
            binVect.push_back(std::stoi(s2, nullptr, 16));
        }else
        {
            binVect.push_back(std::stoi(word, nullptr, 16));
            std::cout << word << "|";
        }
    }
    
    std::cout << "\n";
    file.close();
    return binVect;
}

#endif // HEXTXTTOBIN_HPP