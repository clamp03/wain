#include "common.h"
#include "type.h"

FuncType::FuncType()
        : has_return_type_(false) {
}

void FuncType::setForm(int8_t form) {
    form_ = form;
}

int8_t FuncType::form() {
    return form_;
}

uint32_t FuncType::paramCount() {
    return param_types_.size();
}

void FuncType::addParamType(ValueType type) {
    param_types_.emplace_back(type);
}

ValueType FuncType::paramType(uint32_t idx) {
    return param_types_.at(idx);
}

bool FuncType::hasReturnType() {
    return has_return_type_;
}

void FuncType::setReturnType(ValueType type) {
    return_type_ = type;
    has_return_type_ = true;
}

ValueType FuncType::returnType() {
    return return_type_;
}

GlobalType::GlobalType(ValueType content_type, uint8_t mutability)
        : content_type_(content_type)
        , mutability_(mutability_) {
}

ValueType GlobalType::contentType() {
    return content_type_;
}

uint8_t GlobalType::mutability() {
    return mutability_;
}

TableType::TableType(ElemType element_type, ResizableLimits limits)
        : element_type_(element_type)
        , limits_(limits) {
}

ElemType TableType::elementType() {
    return element_type_;
}

MemoryType::MemoryType(ResizableLimits limits)
        : limits_(limits) {
}

ResizableLimits::ResizableLimits(uint8_t flags, uint32_t initial, uint32_t maximum)
        : flags_(flags)
        , initial_(initial)
        , maximum_(maximum) {
    DEV_ASSERT(flags || !maximum, "Invalid resiable limits");
}
