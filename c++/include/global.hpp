#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <SFML/Graphics.hpp>

extern bool print_debug;
extern sf::Font baseFont;

std::string readFile(std::string f);
void writeHexFile(std::string content, std::string fileName);

#endif // GLOBAL_HPP