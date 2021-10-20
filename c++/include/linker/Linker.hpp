#ifndef LINKER_HPP
#define LINKER_HPP

#include <string>
#include <stdbool.h>
#include <vector>
#include <sstream>

class Linker
{
private:
    struct fileData
    {
        std::string name;
        uint16_t adress;
        uint16_t maxSize;
        uint16_t size;
    };

    struct labelData
    {
        std::string name;
        uint16_t adress;
    };

    bool error;
    unsigned int counter;
    std::vector<fileData> filesData;
    std::vector<labelData> labelsData;

    void loadCompileFiles(std::string linkFile);
    void loadLabelFiles(std::string labelFile);
    void addLabels(std::string fileName);
    void findFileSize();
    std::string linking();

    std::string addToStr(std::string val, std::string string);
    std::string addValToStr(uint8_t val, std::string string);
    std::vector<std::string> split(std::string str, std::string delimiter);
    int findCompileFile(std::string name);
    int findLabel(std::string label);
    std::vector<std::string> readTokens(std::string fileName);
    void sortByAdr();

    void reset();

public:
    Linker();
    ~Linker();

    bool link(std::string linkFile, std::string labelFile, std::string outFileName);
};

#endif // LINKER_HPP