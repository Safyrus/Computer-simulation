#pragma once
#include <string>
#include <vector>

class AssemblerCompiler
{
private:
    std::vector<std::string> instructions;
    std::vector<std::string> labels;
    std::vector<std::string> comments;
    std::vector<int> instructions_pos;
    std::vector<int> labels_pos;
    std::vector<int> labels_val;
    std::vector<int> comments_pos;

    std::vector<std::string> op_name;
    std::vector<int8_t> op_code;
    std::vector<int8_t> op_arg;
    std::vector<std::vector<int8_t>> op_arg_type;
    int8_t op_arg_size;

    std::vector<std::string> reg_name;
    std::vector<int8_t> reg_code;

    char char_comment;
    char char_reg;
    char char_decimal;
    char char_label;
    char char_labelReplace;
    const int8_t LABEL = 2;
    const int8_t REG = 1;
    const int8_t VAL = 0;

    void loadAssembler_read1(const char *f);
    void loadAssembler_read2(const char *f);

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