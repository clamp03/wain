#ifndef TYPE_H 
#define TYPE_H

#include <vector>

#include "common.h"

enum class ValueType {
    NONE = 0,
    I32 = 0x7f,
    I64 = 0x7e,
    F32 = 0x7d,
    F64 = 0x7c
};

class FuncType {
public:
    FuncType();
    void setForm(int8_t form);

    uint32_t getParamCount();
    void addParamType(ValueType type);
    ValueType getParamTypes(uint32_t idx);

    bool hasReturnType();
    void setReturnType(ValueType type);

private:
    int8_t form_;
    std::vector<ValueType> param_types_;

    ValueType return_type_;
};

#endif // TYPE_H
