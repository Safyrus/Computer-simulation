#pragma once
#include <string>
#include <vector>

class AssemblerCompiler
{
private:
    std::vector<int8_t> instructions;
    std::vector<std::string> labels;
    std::vector<std::string> comments;

    std::string *op_name;
    int8_t *op_code;
    int8_t *op_arg;
    int8_t *op_label;
    int8_t **op_label_pos;
    int8_t *op_size;

    std::string *reg_name;
    int8_t *reg_code;

    char char_comment;
    char char_reg;
    char char_decimal;
    char char_label;
    char char_labelReplace;

public:
    AssemblerCompiler();
    AssemblerCompiler(const char *configFile);
    ~AssemblerCompiler();

    void loadAssembler(const char *f);
    void loadBinary(const char *f);
    void saveAssembler(const char *f);
    void saveBinary(const char *f);

    int8_t *getMemory();
};