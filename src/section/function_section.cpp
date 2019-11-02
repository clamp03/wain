#include "common.h"
#include "function_section.h"

uint32_t FunctionSection::count() const {
    return types_.size();
}

void FunctionSection::addFuncType(uint32_t type) {
    types_.emplace_back(type);
}

uint32_t FunctionSection::type(uint32_t idx) const {
    DEV_ASSERT(idx < count(), "Invalid type index");
    return types_[idx];
}
