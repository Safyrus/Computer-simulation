#include "utils/saveString.hpp"
#include "utils/console.hpp"

#include <fstream>

void saveString(std::string fileName, std::string str)
{
    // open the file
    std::ofstream file;
    file.open(fileName);
    if (!file.is_open())
    {
        printError("saveString: open file fail");
        return;
    }

    file << str;
    file.close();
}
