#ifndef TYPE_H 
#define TYPE_H

#include <vector>

#include "section_common.h"

#define I32 0x7F;
#define I64 0x7E;
#define F32 0x7D;
#define F64 0x7C;

#define ANYFUNC 0x70
#define FUNC 0x60
#define BLOCKTYPE 0x40

typedef int8_t ValueType;
typedef int8_t BlockType;
typedef int8_t ElemType;

class FuncType {
public:
    FuncType();
    void setForm(int8_t form);
    int8_t form();

    uint32_t paramCount();
    void addParamType(ValueType type);
    ValueType paramType(uint32_t idx);

    bool hasReturnType();
    void setReturnType(ValueType type);
    ValueType returnType();

private:
    int8_t form_;
    std::vector<ValueType> param_types_;

    uint8_t has_return_type_;
    ValueType return_type_;
};

class ResizableLimits {
public:
    ResizableLimits(uint8_t flags, uint32_t initial, uint32_t maximum = 0);

    uint8_t flags();
    uint32_t initial();
    uint32_t maximum();

private:
    uint8_t flags_;
    uint32_t initial_;
    uint32_t maximum_;
};

class GlobalType {
public:
    GlobalType(ValueType content_type, uint8_t mutability);

    ValueType contentType();
    uint8_t mutability();

private:
    ValueType content_type_;
    uint8_t mutability_;
};

class TableType {
public:
    TableType(ElemType element_type, uint8_t flags, uint32_t initial, uint32_t maximum = 0);

    ElemType elementType();

private:
    ElemType element_type_;
    ResizableLimits limits_;
};

class MemoryType {
public:
    MemoryType(uint8_t flags, uint32_t initial, uint32_t maximum = 0);

private:
    ResizableLimits limits_;
};

#endif // TYPE_H
