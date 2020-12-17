#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <SFML/Graphics.hpp>

extern bool print_debug;
extern sf::Font baseFont;

std::string openFile(std::string f);
void writeFile(std::string content, std::string fileName);

#endif // GLOBAL_HPP