#include <fstream>
#include <iomanip>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "DISK.hpp"
#include "global.hpp"

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
                if (print_debug)
                    std::cout << std::setfill('0') << std::setw(2) << (((int)data[count]) & 0xff) << " ";
                count++;
                if (count % 32 == 0)
                {
                    std::cout << "\n";
                }
            }
        }
    }
    if (print_debug)
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

void DISK::display(sf::RenderWindow &window, int x, int y)
{
    float charSizeX = 8;
    float charSizeY = 10;

    //draw background rect
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(charSizeX*48, charSizeY*16));
    rect.setFillColor(sf::Color(0, 0, 255));
    window.draw(rect);

    //init text
    sf::Text text;
    text.setCharacterSize(20);
    text.setScale(0.5, 0.5);
    text.setFont(baseFont);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(x, y);
    std::stringstream ss ;
    ss << std::hex;

    //get string to display
    int start = (adr/256)*256;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            int at = ((i*16)+j);
            ss << std::uppercase << std::setfill('0') << std::setw(2) << (int)(data[at + start]&0x000000ff) << " ";
        }
        ss << "\n";
    }

    //display text
    text.setString(ss.str());
    window.draw(text);

    //draw selection rect
    int adrX = adr%16;
    int adrY = adr%256/16;
    rect.setPosition(x + charSizeX*adrX*3, y + charSizeY*adrY);
    rect.setSize(sf::Vector2f(charSizeX*3, charSizeY));
    rect.setFillColor(sf::Color(255, 255, 255));
    window.draw(rect);

    // draw selection string
    text.setFillColor(sf::Color(0, 0, 0));
    text.setPosition(x + charSizeX*adrX*3, y + charSizeY*adrY);
    ss.str("");
    ss << std::uppercase << std::setfill('0') << std::setw(2) << (int)(data[adr]&0x000000ff) << " ";
    text.setString(ss.str());
    window.draw(text);
}