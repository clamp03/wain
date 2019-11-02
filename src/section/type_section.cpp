#include "type_section.h"

uint32_t TypeSection::count() const {
    return func_types_.size();
}

const FuncType* TypeSection::funcType(uint32_t idx) const {
    DEV_ASSERT(idx < count(), "Invalid function index");
    return func_types_.at(idx);
}

void TypeSection::addFuncType(const FuncType* func_type) {
    func_types_.emplace_back(func_type);
}
