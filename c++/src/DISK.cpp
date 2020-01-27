#include "DISK.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

DISK::DISK(int l)
{
    len = l;
    adr = 0;
    tab = new int8_t[len];
}

DISK::~DISK()
{
    if (tab != NULL)
    {
        delete tab;
    }
}

int DISK::getAdr()
{
    return adr;
}

void DISK::setAdr(int a)
{
    if (a < len)
    {
        adr = a;
    }
}

int8_t DISK::getData()
{
    return tab[adr];
}

int DISK::getData4()
{
    unsigned int data = 0;
    data |= ((((unsigned)tab[adr + 0]) & 0xff) << 24);
    std::cout << data << " ";
    data |= ((((unsigned)tab[adr + 1]) & 0xff) << 16);
    std::cout << data << " ";
    data |= ((((unsigned)tab[adr + 2]) & 0xff) << 8);
    std::cout << data << " ";
    data |= ((((unsigned)tab[adr + 3]) & 0xff) << 0);
    std::cout << data << " \n";
    return data;
}

void DISK::setData(int8_t d)
{
    tab[adr] = d;
}

int DISK::getLen()
{
    return len;
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
            if (count >= len)
            {
                std::cout << "DISK : ERROR, disk is too small : disk size=" << len << "\n";
                return false;
            }
            if (word != "")
            {
                count++;
            }
        }
    }
    file.close();
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
                tab[count] = stoi(word, NULL, 16);
                std::cout << std::setfill('0') << std::setw(2) << (((int)tab[count]) & 0xff) << " ";
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
        file << std::setfill('0') << std::setw(2) << tab[i] << ' ';
        if (i % numPerLine == 0 && i != 0)
        {
            file << "\n";
        }
    }
    file.close();
    return true;
}
