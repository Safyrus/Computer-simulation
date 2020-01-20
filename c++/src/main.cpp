#include <iostream>
#include <iomanip>
#include "AssemblerCompiler.hpp"

int main() {
    std::string file = "";
    AssemblerCompiler *compiler = new AssemblerCompiler();

    std::cout << "-=#[ Test for assemblerCompiler(loadAssembly) ]#=-" << std::endl;
    std::cout << "--- Enter file name to open ---" << std::endl;
    std::cin >> file;

    compiler->loadAssembler(file.c_str());
    std::cin.ignore();

    std::cout << "\n\n-=#[ Test for assemblerCompiler(saveBinary) ]#=-" << std::endl;
    std::cout << "--- Enter file name to save ---" << std::endl;
    std::cin >> file;

    compiler->saveBinary(file.c_str());

    delete compiler;

    std::cout << "-=#[ Done ]#=-" << std::endl;
    std::cin.ignore();
    std::cin.ignore();
}