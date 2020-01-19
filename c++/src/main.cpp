#include <iostream>
#include "AssemblerCompiler.hpp"

int main() {
    std::cout << "-=#[ Test for assemblerCompiler(loadAssembly) ]#=-" << std::endl;
    std::cin.ignore();

    AssemblerCompiler *compiler = new AssemblerCompiler();
    compiler->loadAssembler("testCompiler.sac");
    std::cout << "\n\n-=#[ Test for assemblerCompiler(saveBinary) ]#=-" << std::endl;
    std::cin.ignore();
    compiler->saveBinary("binary.txt");
    delete compiler;

    std::cout << "-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
}