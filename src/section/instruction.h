#ifndef INSTRUCTION_H 
#define INSTRUCTION_H

#include <vector>

#include "common.h"

#define ENUMERATE_OPCODE(FUNC) \
    FUNC(Block, 0x02)          \
    FUNC(Loop, 0x03)           \
    FUNC(If, 0x04)             \
    FUNC(End, 0x0b)            \
    FUNC(Br, 0x0c)             \
    FUNC(BrIf, 0x0d)           \
    FUNC(BrTable, 0x0e)        \
    FUNC(Call, 0x10)           \
    FUNC(CallIndirect, 0x11)   \
    FUNC(GetLocal, 0x20)       \
    FUNC(SetLocal, 0x21)       \
    FUNC(TeeLocal, 0x22)       \
    FUNC(GetGlobal, 0x23)      \
    FUNC(SetGlobal, 0x24)      \
    FUNC(I32Load, 0x28)        \
    FUNC(I64Load, 0x29)        \
    FUNC(F32Load, 0x2a)        \
    FUNC(F64Load, 0x2b)        \
    FUNC(I32Load8S, 0x2c)      \
    FUNC(I32Load8U, 0x2d)      \
    FUNC(I32Load16S, 0x2e)     \
    FUNC(I32Load16U, 0x2f)     \
    FUNC(I64Load8S, 0x30)      \
    FUNC(I64Load8U, 0x31)      \
    FUNC(I64Load16S, 0x32)     \
    FUNC(I64Load16U, 0x33)     \
    FUNC(I64Load32S, 0x34)     \
    FUNC(I64Load32U, 0x35)     \
    FUNC(I32Store, 0x36)       \
    FUNC(I64Store, 0x37)       \
    FUNC(F32Store, 0x38)       \
    FUNC(F64Store, 0x39)       \
    FUNC(I32Store8, 0x3a)      \
    FUNC(I32Store16, 0x3b)     \
    FUNC(I64Store8, 0x3c)      \
    FUNC(I64Store16, 0x3d)     \
    FUNC(I64Store32, 0x3e)     \
    FUNC(CurrentMemory, 0x3f)  \
    FUNC(GrowMemory, 0x40)     \
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
    virtual const char* opcodeName() const = 0;
};

class Block : public Instruction { // 0x02
public:
    Block(int8_t type);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    int8_t type_;
};

class Loop : public Instruction { // 0x03
public:
    Loop(int8_t type);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    int8_t type_;
};

class If : public Instruction { // 0x04
public:
    If(int8_t type);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    int8_t type_;
};

class End : public Instruction { // 0x0b
public:
    End();
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class Br : public Instruction { // 0x0c
public:
    Br(uint32_t depth);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t depth_;
};

class BrIf : public Instruction { // 0x0d
public:
    BrIf(uint32_t depth);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t depth_;
};

class BrTable : public Instruction { // 0x0e
public:
    uint32_t count();
    void addTargetTable(uint32_t target);
    void setDefaultTarget(uint32_t target);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    std::vector<uint32_t> targets_;
    uint32_t default_target_;
};

class Call : public Instruction { // 0x10
public:
    Call(uint32_t index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

    uint32_t index() { return index_; }

private:
    uint32_t index_;
};

class CallIndirect : public Instruction { // 0x11
public:
    CallIndirect(uint32_t type_index, int8_t reserved);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t type_index_;
    int8_t reserved_;
};

class GetLocal : public Instruction { // 0x20
public:
    GetLocal(uint32_t local_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t local_index_;
};

class SetLocal : public Instruction { // 0x21
public:
    SetLocal(uint32_t local_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t local_index_;
};

class TeeLocal : public Instruction { // 0x22
public:
    TeeLocal(uint32_t local_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t local_index_;
};

class GetGlobal : public Instruction { // 0x23
public:
    GetGlobal(uint32_t global_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t global_index_;
};

class SetGlobal : public Instruction { // 0x24
public:
    SetGlobal(uint32_t global_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t global_index_;
};

class I32Load : public Instruction { // 0x28
public:
    I32Load(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load : public Instruction { // 0x29
public:
    I64Load(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class F32Load : public Instruction { // 0x2a
public:
    F32Load(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class F64Load : public Instruction { // 0x2b
public:
    F64Load(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Load8S : public Instruction { // 0x2c
public:
    I32Load8S(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Load8U : public Instruction { // 0x2d
public:
    I32Load8U(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Load16S : public Instruction { // 0x2e
public:
    I32Load16S(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Load16U : public Instruction { // 0x2f
public:
    I32Load16U(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load8S : public Instruction { // 0x30
public:
    I64Load8S(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load8U : public Instruction { // 0x31
public:
    I64Load8U(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load16S : public Instruction { // 0x32
public:
    I64Load16S(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load16U : public Instruction { // 0x33
public:
    I64Load16U(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load32S : public Instruction { // 0x34
public:
    I64Load32S(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Load32U : public Instruction { // 0x35
public:
    I64Load32U(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Store : public Instruction { // 0x36
public:
    I32Store(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Store : public Instruction { // 0x37
public:
    I64Store(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class F32Store : public Instruction { // 0x38
public:
    F32Store(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class F64Store : public Instruction { // 0x39
public:
    F64Store(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Store8 : public Instruction { // 0x3a
public:
    I32Store8(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I32Store16 : public Instruction { // 0x3b
public:
    I32Store16(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Store8 : public Instruction { // 0x3c
public:
    I64Store8(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Store16 : public Instruction { // 0x3d
public:
    I64Store16(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class I64Store32 : public Instruction { // 0x3e
public:
    I64Store32(uint32_t flags, uint32_t offset);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t flags_;
    uint32_t offset_;
};

class CurrentMemory : public Instruction { // 0x3f
public:
    CurrentMemory(uint8_t reserved);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint8_t reserved_;
};

class GrowMemory : public Instruction { // 0x40
public:
    GrowMemory(uint8_t reserved);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint8_t reserved_;
};

class I32Const : public Instruction { // 0x41
public:
    I32Const(int32_t value);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    int32_t value_;
};

class I64Const : public Instruction { // 0x42
public:
    I64Const(int64_t value);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    int64_t value_;
};

class F32Const : public Instruction { // 0x43
public:
    F32Const(uint32_t value);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint32_t value_;
};

class F64Const : public Instruction { // 0x44
public:
    F64Const(uint64_t value);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

private:
    uint64_t value_;
};

#endif // INSTRUCTION_H
