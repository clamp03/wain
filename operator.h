#ifndef OPERATOR_H 
#define OPERATOR_H

#include "common.h"

class Operator {
public:
    virtual uint8_t opcode() const = 0;
};

class EndOperator : public Operator { // 0x0b
public:
    EndOperator();
    virtual uint8_t opcode() const;
};

class GetGlobalOperator : public Operator { // 0x23
public:
    GetGlobalOperator(uint32_t global_index);
    virtual uint8_t opcode() const;

private:
    uint32_t global_index_;
};

class I32ConstOperator : public Operator { // 0x41
public:
    I32ConstOperator(int32_t value);
    virtual uint8_t opcode() const;

private:
    int32_t value_;
};

class I64ConstOperator : public Operator { // 0x42
public:
    I64ConstOperator(int64_t value);
    virtual uint8_t opcode() const;

private:
    int64_t value_;
};

class F32ConstOperator : public Operator { // 0x43
public:
    F32ConstOperator(uint32_t value);
    virtual uint8_t opcode() const;

private:
    uint32_t value_;
};

class F64ConstOperator : public Operator { // 0x44
public:
    F64ConstOperator(uint64_t value);
    virtual uint8_t opcode() const;

private:
    uint64_t value_;
};

#endif // OPERATOR_H
