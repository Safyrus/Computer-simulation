#include <iostream>
#include "AssemblerCompiler.hpp"

int main() {
    std::cout << "Test for assemblerCompiler" << std::endl;
    std::cin.ignore();

    AssemblerCompiler *compiler = new AssemblerCompiler();
    compiler->loadAssembler("testCompiler.sac");
    delete compiler;

    std::cout << "Done" << std::endl;
    std::cin.ignore();
}