#include "DISK.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

DISK::DISK(int l) : Device(l)
{
}

DISK::~DISK()
{
}

bool DISK::load(const char *f)
{
    std::ifstream file(f);
    if (!file.is_open())
    {
        std::cout << "DISK : ERROR, can't load file\n";
        return false;
    }

    int count = 0;
    bool size = true;
    std::string line;
    while (getline(file, line))
    {
        std::string word = "";
        while (line != "")
        {
            word = line.substr(0, line.find(' '));
            line = line.substr(line.find(' ') + 1);
            //std::cout << word << '|' << line << "]";
            //std::cin.ignore();
            if (count > len)
            {
                size = false;
            }
            if (word != "")
            {
                count++;
            }
        }
    }
    file.close();
    if (!size)
    {
        std::cout << "DISK : ERROR, disk is too small : disk size=" << len << "  file size=" << count << "\n";
        return false;
    }
    file.open(f);
    count = 0;
    while (getline(file, line))
    {
        std::string word = "";
        while (line != "")
        {
            word = line.substr(0, line.find(' '));
            line = line.substr(line.find(' ') + 1);
            if (word != "")
            {
                data[count] = stoi(word, NULL, 16);
                std::cout << std::setfill('0') << std::setw(2) << (((int)data[count]) & 0xff) << " ";
                count++;
                if (count % 32 == 0)
                {
                    std::cout << "\n";
                }
            }
        }
    }
    std::cout << "\n";
    file.close();
    return true;
}

bool DISK::save(const char *f)
{
    std::ofstream file(f);
    if (!file.is_open())
    {
        return false;
    }

    int numPerLine = 8;
    file << std::hex;
    for (int i = 0; i < len; i++)
    {
        file << std::setfill('0') << std::setw(2) << data[i] << ' ';
        if (i % numPerLine == 0 && i != 0)
        {
            file << "\n";
        }
    }
    file.close();
    return true;
}
