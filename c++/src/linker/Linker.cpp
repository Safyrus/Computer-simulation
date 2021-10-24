#include "linker/Linker.hpp"

#include "utils/console.hpp"
#include "utils/hexTxtToBin.hpp"
#include "utils/saveString.hpp"
#include "utils/getDir.hpp"
#include "utils/split.hpp"

#include "assembler/Lexer.hpp"

#include <fstream>
#include <iomanip>

Linker::Linker()
{
    reset();
}

Linker::~Linker()
{
}

bool Linker::link(std::string linkFile, std::string labelFile, std::string outFileName)
{
    printInfo("Linker: Load compile files ...");
    loadCompileFiles(linkFile);
    if (error)
    {
        printError("Linker: Error during linkfile reading, cannot continue");
        return error;
    }
    printInfo("OK");

    printInfo("Linker: Load label files ...");
    loadLabelFiles(labelFile);
    if (error)
    {
        printError("Linker: Error during labelFile reading, cannot continue");
        return error;
    }
    printInfo("OK");

    printInfo("Linker: Find files sizes ...");
    findFileSize();
    if (error)
    {
        printError("Linker: Error finding file size, cannot continue");
        return error;
    }
    printInfo("OK");

    printInfo("Linker: Linking ...");
    std::string str = linking();
    if (error)
    {
        printError("Linker: Error during linknig, cannot continue");
        return error;
    }
    printInfo("OK");

    saveString(outFileName, str);

    return error;
}

void Linker::loadCompileFiles(std::string linkFile)
{
    // open the file
    std::ifstream file;
    file.open(linkFile);
    if (!file.is_open())
    {
        printError("Linker: Cannot open link file " + linkFile);
        error = true;
        return;
    }

    std::string dir = getDir(linkFile);

    // read each lines
    std::string line;
    while (std::getline(file, line))
    {
        // split the line into words
        std::vector<std::string> tokens = split(line, " ");

        if (tokens.size() < 3)
        {
            printError("Linker: Wrong number of arguments");
            error = true;
            continue;
        }
        if (findCompileFile(tokens[0]) != -1)
        {
            printError("Linker: Cannot add link file " + tokens[0] + " multiple times");
            error = true;
        }
        else
        {
            fileData data;
            data.name = dir + tokens[0];
            printDebug("Linker: Find file " + data.name, true);
            data.adress = std::stoi(tokens[1], nullptr, 16);
            printDebug("  " + std::to_string(data.adress), true);
            data.maxSize = std::stoi(tokens[2], nullptr, 16);
            printDebug("  " + std::to_string(data.maxSize) + "\n", true);
            filesData.push_back(data);
        }
    }
}

void Linker::loadLabelFiles(std::string labelFile)
{
    // open the file
    std::ifstream file;
    file.open(labelFile);
    if (!file.is_open())
    {
        printError("Linker: Cannot open label file " + labelFile);
        error = true;
        return;
    }

    std::string dir = getDir(labelFile);

    // read each lines
    std::string line;
    while (std::getline(file, line))
    {
        // split the line into words
        std::vector<std::string> tokens = split(line, " ");

        if (tokens.size() < 1)
        {
            continue;
        }

        addLabels(dir + tokens[0]);
    }
}

void Linker::addLabels(std::string fileName)
{
    // open the file
    std::ifstream file;
    file.open(fileName);
    if (!file.is_open())
    {
        printError("Linker: Cannot open sasm file " + fileName);
        error = true;
        return;
    }

    // read each lines
    std::string line;
    while (std::getline(file, line))
    {
        // split the line into words
        std::vector<std::string> tokens = split(line, " ");

        if (tokens.size() < 2)
        {
            printError("Linker: Wrong number of arguments");
            error = true;
            continue;
        }
        if (findLabel(tokens[0]) != -1)
        {
            printError("Linker: Cannot add label " + tokens[0] + " multiple times");
            error = true;
        }
        else
        {
            labelData data;
            data.name = tokens[0];
            printDebug("Linker: Find label " + data.name, true);
            data.adress = std::stoi(tokens[1], nullptr, 16);
            printDebug("  " + std::to_string(data.adress) + "\n", true);
            labelsData.push_back(data);
        }
    }
}

void Linker::findFileSize()
{
    for (unsigned int i = 0; i < filesData.size(); i++)
    {
        fileData fileData = filesData[i];

        std::vector<std::string> tokens = readTokens(fileData.name);
        fileData.size = tokens.size();
        printDebug("File " + fileData.name + " of size " + std::to_string(fileData.size) + "  " + std::to_string(fileData.maxSize));

        if (fileData.size > fileData.maxSize)
        {
            printError("Linker: File " + fileData.name + " is too big by " + std::to_string(fileData.maxSize - fileData.size));
            error = true;
        }
        filesData[i] = fileData;
    }
}

std::string Linker::linking()
{
    std::string out;

    sortByAdr();

    for (unsigned int i = 0; i < filesData.size(); i++)
    {
        printInfo("Name: " + filesData[i].name + "  adr: " + std::to_string(filesData[i].adress));
    }

    for (unsigned int i = 0; i < filesData[0].adress; i++)
    {
        out = addToStr("00", out);
    }

    for (unsigned int i = 0; i < filesData.size(); i++)
    {
        fileData fileData = filesData[i];
        std::vector<std::string> tokens = readTokens(fileData.name);
        for (unsigned int j = 0; j < tokens.size(); j++)
        {
            if (tokens[j][0] == Lexer::CHAR_LABEL)
            {
                std::string str = tokens[j].substr(1, tokens[j].find_last_of(":") - 1);
                int index = findLabel(str);
                if (index == -1)
                {
                    printError("Linker: Label " + str + " does not exist");
                    error = true;
                    continue;
                }

                uint8_t val = 0;
                char c = tokens[j][tokens[j].size() - 1];
                if (c == Lexer::CHAR_LABEL_HIGH)
                {
                    val = (labelsData[index].adress / 256);
                }
                else
                {
                    val = (labelsData[index].adress % 256);
                }
                out = addValToStr(val, out);
            }
            else
            {
                out = addToStr(tokens[j], out);
            }
        }
        int pad = fileData.maxSize - tokens.size();
        if (pad > 0)
        {
            for (int j = 0; j < pad; j++)
            {
                out = addToStr("00", out);
            }
        }
    }

    return out;
}

std::string Linker::addValToStr(uint8_t val, std::string string)
{
    std::stringstream sstr;
    sstr << string;

    sstr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)val) << " ";
    counter++;
    if (counter % 16 == 0)
    {
        sstr << "\n";
    }
    else if (counter % 4 == 0)
    {
        sstr << " ";
    }

    return sstr.str();
}

std::string Linker::addToStr(std::string val, std::string string)
{
    std::stringstream sstr;
    sstr << string;

    sstr << val << " ";
    counter++;
    if (counter % 16 == 0)
    {
        sstr << "\n";
    }
    else if (counter % 4 == 0)
    {
        sstr << " ";
    }

    return sstr.str();
}

int Linker::findCompileFile(std::string name)
{
    for (unsigned int i = 0; i < filesData.size(); i++)
    {
        fileData data = filesData[i];
        if (data.name.compare(name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int Linker::findLabel(std::string label)
{
    for (unsigned int i = 0; i < labelsData.size(); i++)
    {
        labelData data = labelsData[i];
        if (data.name.compare(label) == 0)
        {
            return i;
        }
    }
    return -1;
}

std::vector<std::string> Linker::readTokens(std::string fileName)
{
    std::vector<std::string> tokens;

    // open the file
    std::ifstream file;
    file.open(fileName);
    if (!file.is_open())
    {
        printError("Linker: Cannot open label file " + fileName);
        error = true;
        return tokens;
    }

    // read each lines
    std::string line;
    while (std::getline(file, line))
    {
        // split the line into words
        std::vector<std::string> lineTokens = split(line, " ");
        for (unsigned int i = 0; i < lineTokens.size(); i++)
        {
            if (lineTokens[i].compare("") != 0)
            {
                tokens.push_back(lineTokens[i]);
            }
        }
    }

    return tokens;
}

void Linker::sortByAdr()
{
    for (int i = filesData.size(); i >= 0; i--)
    {
        bool sorted = true;

        for (int j = 0; j < i - 1; j++)
        {
            if (filesData[j + 1].adress < filesData[j].adress)
            {
                fileData tmp = filesData[j + 1];
                filesData[j + 1] = filesData[j];
                filesData[j] = tmp;
                sorted = false;
            }
        }

        if (sorted)
        {
            break;
        }
    }
}

void Linker::reset()
{
    error = false;
    counter = 0;
    filesData.clear();
    labelsData.clear();
}
