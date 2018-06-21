#include "type.h"

FuncType::FuncType()
        : return_type_(ValueType::NONE) {
}

void FuncType::setForm(int8_t form) {
    form_ = form;
}

uint32_t FuncType::getParamCount() {
    return param_types_.size();
}

void FuncType::addParamType(ValueType type) {
    param_types_.emplace_back(type);
}

ValueType FuncType::getParamTypes(uint32_t idx) {
    return param_types_.at(idx);
}

bool FuncType::hasReturnType() {
    return return_type_ != ValueType::NONE;
}

void FuncType::setReturnType(ValueType type) {
    return_type_ = type;
}
