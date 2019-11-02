#ifndef FUNCTION_SECTION_H
#define FUNCTION_SECTION_H

#include <vector>

class FunctionSection {
public:
    uint32_t count() const;
    void addFuncType(uint32_t type);
    uint32_t type(uint32_t idx) const;

private:
    std::vector<uint32_t> types_;
};

#endif // FUNCTION_SECTION_H
