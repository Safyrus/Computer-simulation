#include "utils/hexTxtToBin.hpp"
#include "utils/console.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<unsigned char> hexTxtToBin(std::string textFilePath)
{
    // data to return
    std::vector<unsigned char> binVect;

    // open the file
    std::ifstream file;
    file.open(textFilePath);
    if (!file.is_open())
    {
        printError("hexTxtToBin: open file fail");
        return binVect;
    }

    // read each 'word' of the file
    std::string word;
    while (std::getline(file, word, ' '))
    {
        // if there is a line feed character in the word
        if (word.find('\n') != std::string::npos)
        {
            // then separate the word in 2
            std::string s1 = word.substr(0, word.find('\n'));
            std::string s2 = word.substr(word.find('\n') + 1);
            printDebug(s1 + "|" + s2 + "|", true);

            // convert the first word
            try
            {
                binVect.push_back(std::stoi(s1, nullptr, 16));
            }
            catch (const std::invalid_argument &ia)
            {
                if(s1 != "\n" && s1 != "")
                {
                    printWarning("hexTxtToBin: cannot convert \"" + s1 + "\"");
                }
            }
            catch (const std::exception &e)
            {
            }

            // convert the second word
            try
            {
                binVect.push_back(std::stoi(s2, nullptr, 16));
            }
            catch (const std::invalid_argument &ia)
            {
                if(s2 != "\n" && s2 != "")
                {
                    printWarning("hexTxtToBin: cannot convert \"" + s2 + "\"");
                }
            }
            catch (const std::exception &e)
            {
            }
        }
        else
        {
            try
            {
                // convert the word
                binVect.push_back(std::stoi(word, nullptr, 16));
                printDebug(word + "|", true);
            }
            catch (const std::invalid_argument &ia)
            {
                if(word != "")
                {
                    printWarning("hexTxtToBin: cannot convert \"" + word + "\"");
                }
            }
            catch (const std::exception &e)
            {
            }
        }
    }

    // close the file and return data
    printDebug("\n", true);
    file.close();
    return binVect;
}
