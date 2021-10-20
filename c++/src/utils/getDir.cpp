#include "utils/getDir.hpp"

std::string getDir(std::string fileName)
{
    std::string sep = "/";
    unsigned int index = fileName.find_last_of(sep);
    if (std::string::npos != index)
    {
        return fileName.substr(0, index+1);
    }
    return "";
}
