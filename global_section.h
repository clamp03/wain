#ifndef GLOBAL_SECTION_H
#define GLOBAL_SECTION_H

#include <vector>

#include "init.h"
#include "type.h"

class GlobalEntry {
public:
    GlobalEntry(ValueType content_type, uint8_t mutability, InitExpr* init);

private:
    GlobalType type_;
    InitExpr* init_;
};

class GlobalSection {
public:
    uint32_t count();
    void addGlobalVariable(GlobalEntry* variable);

private:
    std::vector<GlobalEntry*> globals_;
};

#endif // GLOBAL_SECTION_H
