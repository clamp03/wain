#ifndef INSTRUCTION_H 
#define INSTRUCTION_H

#include "common.h"

#define ENUMERATE_OPCODE(FUNC) \
    FUNC(End, 0x0b)            \
    FUNC(GetGlobal, 0x23)      \
    FUNC(I32Const, 0x41)       \
    FUNC(I64Const, 0x42)       \
    FUNC(F32Const, 0x43)       \
    FUNC(F64Const, 0x44)

#define DEFINE_OPCODE(name, code) const unsigned name##Opcode = code;
ENUMERATE_OPCODE(DEFINE_OPCODE)
#undef DEFINE_OPCODE

class Instruction {
public:
    virtual uint8_t opcode() const = 0;
};

class End : public Instruction { // 0x0b
public:
    End();
    virtual uint8_t opcode() const;
};

class GetGlobal : public Instruction { // 0x23
public:
    GetGlobal(uint32_t global_index);
    virtual uint8_t opcode() const;

private:
    uint32_t global_index_;
};

class I32Const : public Instruction { // 0x41
public:
    I32Const(int32_t value);
    virtual uint8_t opcode() const;

private:
    int32_t value_;
};

class I64Const : public Instruction { // 0x42
public:
    I64Const(int64_t value);
    virtual uint8_t opcode() const;

private:
    int64_t value_;
};

class F32Const : public Instruction { // 0x43
public:
    F32Const(uint32_t value);
    virtual uint8_t opcode() const;

private:
    uint32_t value_;
};

class F64Const : public Instruction { // 0x44
public:
    F64Const(uint64_t value);
    virtual uint8_t opcode() const;

private:
    uint64_t value_;
};

#endif // INSTRUCTION_H
