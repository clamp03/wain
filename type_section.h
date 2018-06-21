#ifndef TYPE_SECTION_H 
#define TYPE_SECTION_H

#include <vector>

#include "common.h"
#include "section.h"
#include "type.h"

class TypeSection {
public:
    uint32_t getTypeCount();
    void addFuncType(const FuncType* func_type);

private:
    uint32_t type_count_;
    std::vector<const FuncType*> func_types_;
};

#endif // TYPE_SECTION_H
