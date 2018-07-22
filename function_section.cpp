#include "common.h"
#include "function_section.h"

uint32_t FunctionSection::count() {
    return types_.size();
}

void FunctionSection::addFuncType(uint32_t type) {
    types_.emplace_back(type);
}
