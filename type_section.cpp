#include "type_section.h"

uint32_t TypeSection::getTypeCount() {
    return func_types_.size();
}

void TypeSection::addFuncType(const FuncType* func_type) {
    func_types_.emplace_back(func_type);
}
