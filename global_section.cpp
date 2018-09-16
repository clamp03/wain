#include "common.h"
#include "global_section.h"

GlobalEntry::GlobalEntry(ValueType content_type, uint8_t mutability, InitExpr* init)
        : type_(content_type, mutability)
        , init_(init) {
}

uint32_t GlobalSection::count() {
    return globals_.size();
}

void GlobalSection::addGlobalVariable(GlobalEntry* variable) {
    globals_.emplace_back(variable);
}
