#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Screen.hpp"

Screen::Screen()
{
    len = W*H;
    adr = 0;
    data = new int8_t[len];
    for (int i = 0; i < len; i++)
    {
        data[i] = 0;
    }
    data[0] = W;
    data[1] = H;

    RT.create(W, H);

    pixMat.resize(W*H);
    for (int i=0;i<H;++i)
            for (int j=0;j<W;++j)
                    pixMat[(i*W)+j] = sf::Vertex(sf::Vector2f(j+.5f, i+.5f), sf::Color(0, 0, 0));
    
    color[0] = 0x00;
    color[1] = 0x03;
    color[2] = 0x0c;
    color[3] = 0x0f;
    color[4] = 0x30;
    color[5] = 0x33;
    color[6] = 0x3c;
    color[7] = 0x3f;
    color[8] = 0xc0;
    color[9] = 0xc3;
    color[10] = 0xcc;
    color[11] = 0xcf;
    color[12] = 0xf0;
    color[13] = 0xf3;
    color[14] = 0xfc;
    color[15] = 0xff;
}

Screen::~Screen()
{
}

void Screen::setData(int8_t d)
{
    std::cout << (int)d << "\n" << std::flush;
    if(adr>15)
    {
        unsigned int ind = data[3]&0xff;
        data[adr*ind] = d;
        std::cout << (int)(data[adr*ind]) << " " << adr << std::flush;
        int colorIndex;
        unsigned int dat = data[adr];
        if(adr%2==0)
        {
            colorIndex = ((data[adr*ind]&0xf0)>>4);
        }else
        {
            colorIndex = data[adr*ind]&0x0f;
        }
        int r = ((color[colorIndex]&0xc0)>>6)*43;
        int g = ((color[colorIndex]&0x30)>>4)*43;
        int b = ((color[colorIndex]&0x0c)>>2)*43;
        int a = ((color[colorIndex]&0x03)>>0)*43;
        if(r>0x7f)
        {
            r=0x7f;
        }
        if(g>0x7f)
        {
            g=0x7f;
        }
        if(b>0x7f)
        {
            b=0x7f;
        }
        if(a>0x7f)
        {
            a=0x7f;
        }

        std::cout << "color: " << colorIndex << " " << a << " " << r+a << " " << g+a << " " << b+a << "\n" << std::flush;

        switch (data[2])
        {
        case 0:
            std::cout << (int)(adr-16) << " " << (int)(ind) << " " << (int)(((adr-16)*W)+ind) << "\n" << std::flush;
            pixMat[(((adr-16)*W)+ind)] = sf::Vertex(sf::Vector2f(ind+.5f, (adr-16)+.5f), sf::Color(r+a, g+a, b+a));
            break;
        }
    }else if(adr==5)
    {
        color[data[4]] = d;
    }else
    {
        data[adr] = d;
    }
}

int8_t Screen::getData()
{
    if(adr>15)
    {
        return data[adr*data[3]];
    }
    return data[adr];
}

void Screen::setAdr(int a)
{
    adr = a%256;
}

void Screen::print(int x, int y)
{
    int resX = data[0];
    int resY = data[1];
    int mode = data[2];
    int count = 3;
    std::stringstream ss;
    ss << "\x1b[" << y << ";" << x << "H";
    std::cout << ss.str();

    switch (mode)
    {
    case 0:
        for (int j = 0; j < resY; j++)
        {
            ss.clear();
            ss << "\x1b[" << y + j << ";" << x << "H";
            for (int i = 0; i < resX; i++)
            {
                if (count >= len)
                {
                    std::cout << ss.str();
                    break;
                }
                if (data[count] != 0)
                    ss << "\x1b[102m";
                else
                    ss << "\x1b[42m";
                ss << ' ';
                count++;
            }
            std::cout << ss.str();
        }
        break;
    }
    ss.clear();
    ss << "\x1b[0m";
    std::cout << ss.str();
    std::cout << std::flush;
}

void Screen::display(sf::RenderWindow &window, int x, int y)
{
    int resX = data[0];
    int resY = data[1];
    int mode = data[2];
    int index = data[3];
    int indexColor = data[4];
    //data[5] = color

    //Draws the pixel matrix
    RT.draw(pixMat.data(), H*W, sf::Points);

    //And finally draws the RenderTexture to the RenderWindow
    RT.display();
    sf::Sprite spr(RT.getTexture());
    spr.setPosition((float)x, (float)y);
    window.draw(spr);
}
