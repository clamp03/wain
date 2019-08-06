#ifndef FUNCTION_SECTION_H
#define FUNCTION_SECTION_H

#include <vector>

class FunctionSection {
public:
    uint32_t count();
    void addFuncType(uint32_t type);
    uint32_t type(uint32_t idx);

private:
    std::vector<uint32_t> types_;
};

#endif // FUNCTION_SECTION_H
