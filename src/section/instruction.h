#ifndef INSTRUCTION_H 
#define INSTRUCTION_H

#include <vector>

#include "common.h"

#define ENUMERATE_OPCODE(FUNC)     \
    FUNC(Unreachable, 0x00)        \
    FUNC(Nop, 0x01)                \
    FUNC(Block, 0x02)              \
    FUNC(Loop, 0x03)               \
    FUNC(If, 0x04)                 \
    FUNC(Else, 0x05)               \
    FUNC(End, 0x0b)                \
    FUNC(Br, 0x0c)                 \
    FUNC(BrIf, 0x0d)               \
    FUNC(BrTable, 0x0e)            \
    FUNC(Return, 0xf)              \
    FUNC(Call, 0x10)               \
    FUNC(CallIndirect, 0x11)       \
    FUNC(Drop, 0x1a)               \
    FUNC(GetLocal, 0x20)           \
    FUNC(SetLocal, 0x21)           \
    FUNC(TeeLocal, 0x22)           \
    FUNC(GetGlobal, 0x23)          \
    FUNC(SetGlobal, 0x24)          \
    FUNC(I32Load, 0x28)            \
    FUNC(I64Load, 0x29)            \
    FUNC(F32Load, 0x2a)            \
    FUNC(F64Load, 0x2b)            \
    FUNC(I32Load8S, 0x2c)          \
    FUNC(I32Load8U, 0x2d)          \
    FUNC(I32Load16S, 0x2e)         \
    FUNC(I32Load16U, 0x2f)         \
    FUNC(I64Load8S, 0x30)          \
    FUNC(I64Load8U, 0x31)          \
    FUNC(I64Load16S, 0x32)         \
    FUNC(I64Load16U, 0x33)         \
    FUNC(I64Load32S, 0x34)         \
    FUNC(I64Load32U, 0x35)         \
    FUNC(I32Store, 0x36)           \
    FUNC(I64Store, 0x37)           \
    FUNC(F32Store, 0x38)           \
    FUNC(F64Store, 0x39)           \
    FUNC(I32Store8, 0x3a)          \
    FUNC(I32Store16, 0x3b)         \
    FUNC(I64Store8, 0x3c)          \
    FUNC(I64Store16, 0x3d)         \
    FUNC(I64Store32, 0x3e)         \
    FUNC(CurrentMemory, 0x3f)      \
    FUNC(GrowMemory, 0x40)         \
    FUNC(I32Const, 0x41)           \
    FUNC(I64Const, 0x42)           \
    FUNC(F32Const, 0x43)           \
    FUNC(F64Const, 0x44)           \
    FUNC(I32EQZ, 0x45)             \
    FUNC(I32EQ, 0x46)              \
    FUNC(I32NE, 0x47)              \
    FUNC(I32LT_S, 0x48)            \
    FUNC(I32LT_U, 0x49)            \
    FUNC(I32GT_S, 0x4a)            \
    FUNC(I32GT_U, 0x4b)            \
    FUNC(I32LE_S, 0x4c)            \
    FUNC(I32LE_U, 0x4d)            \
    FUNC(I32GE_S, 0x4e)            \
    FUNC(I32GE_U, 0x4f)            \
    FUNC(I64EQZ, 0x50)             \
    FUNC(I64EQ, 0x51)              \
    FUNC(I64NE, 0x52)              \
    FUNC(I64LT_S, 0x53)            \
    FUNC(I64LT_U, 0x54)            \
    FUNC(I64GT_S, 0x55)            \
    FUNC(I64GT_U, 0x56)            \
    FUNC(I64LE_S, 0x57)            \
    FUNC(I64LE_U, 0x58)            \
    FUNC(I64GE_S, 0x59)            \
    FUNC(I64GE_U, 0x5a)            \
    FUNC(F64EQ, 0x61)              \
    FUNC(F64NE, 0x62)              \
    FUNC(I32Add, 0x6a)             \
    FUNC(I32Sub, 0x6b)             \
    FUNC(I32Mul, 0x6c)             \
    FUNC(I32Div_S, 0x6d)           \
    FUNC(I32Div_U, 0x6e)           \
    FUNC(I32Rem_S, 0x6f)           \
    FUNC(I32Rem_U, 0x70)           \
    FUNC(I32And, 0x71)             \
    FUNC(I32Or, 0x72)              \
    FUNC(I32Xor, 0x73)             \
    FUNC(I32Shl, 0x74)             \
    FUNC(I32Shr_S, 0x75)           \
    FUNC(I32Shr_U, 0x76)           \
    FUNC(I64Add, 0x7c)             \
    FUNC(I64Sub, 0x7d)             \
    FUNC(I64Mul, 0x7e)             \
    FUNC(I64Div_S, 0x7f)           \
    FUNC(I64Div_U, 0x80)           \
    FUNC(I64And, 0x83)             \
    FUNC(I64Or, 0x84)              \
    FUNC(I64Xor, 0x85)             \
    FUNC(I64Shl, 0x86)             \
    FUNC(I64Shr_S, 0x87)           \
    FUNC(I64Shr_U, 0x88)           \
    FUNC(F64Neg, 0x9a)             \
    FUNC(F64Add, 0xa0)             \
    FUNC(F64Sub, 0xa1)             \
    FUNC(F64Mul, 0xa2)             \
    FUNC(I32Wrap_I64, 0xa7)        \
    FUNC(I32Trunc_S_F64, 0xaa)     \
    FUNC(I32Trunc_U_F64, 0xab)     \
    FUNC(I64Extend_S_I32, 0xac)    \
    FUNC(I64Extend_U_I32, 0xad)    \
    FUNC(F64Convert_S_I32, 0xb7)   \
    FUNC(F64Convert_U_I32, 0xb8)   \
    FUNC(I64Reinterpret_F64, 0xbd) \
    FUNC(F64Reinterpret_I64, 0xbf)

#define DEFINE_OPCODE(name, code) const unsigned name##Opcode = code;
ENUMERATE_OPCODE(DEFINE_OPCODE)
#undef DEFINE_OPCODE

class Instruction {
public:
    virtual uint8_t opcode() const = 0;
    virtual const char* opcodeName() const = 0;
};

class Unreachable : public Instruction { // 0x00
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class Nop : public Instruction { // 0x01
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
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

class Else : public Instruction { // 0x05
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

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

class Return : public Instruction { // 0x0f
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
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

class Drop : public Instruction { // 0x1a
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class GetLocal : public Instruction { // 0x20
public:
    GetLocal(uint32_t local_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

    uint32_t index() { return local_index_; }

private:
    uint32_t local_index_;
};

class SetLocal : public Instruction { // 0x21
public:
    SetLocal(uint32_t local_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

    uint32_t index() { return local_index_; }

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

    uint32_t index() { return global_index_; }

private:
    uint32_t global_index_;
};

class SetGlobal : public Instruction { // 0x24
public:
    SetGlobal(uint32_t global_index);
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;

    uint32_t index() { return global_index_; }

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

    int32_t value() { return value_; }

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

class I32EQZ : public Instruction { // 0x45
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32EQ : public Instruction { // 0x46
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32NE : public Instruction { // 0x47
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32LT_S : public Instruction { // 0x48
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32LT_U : public Instruction { // 0x49
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32GT_S : public Instruction { // 0x4a
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32GT_U : public Instruction { // 0x4b
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32LE_S : public Instruction { // 0x4c
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32LE_U : public Instruction { // 0x4d
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32GE_S : public Instruction { // 0x4e
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32GE_U : public Instruction { // 0x4f
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64EQZ : public Instruction { // 0x50
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64EQ : public Instruction { // 0x51
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64NE : public Instruction { // 0x52
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64LT_S : public Instruction { // 0x53
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64LT_U : public Instruction { // 0x54
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64GT_S : public Instruction { // 0x55
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64GT_U : public Instruction { // 0x56
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64LE_S : public Instruction { // 0x57
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64LE_U : public Instruction { // 0x58
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64GE_S : public Instruction { // 0x59
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64GE_U : public Instruction { // 0x5a
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64EQ : public Instruction { // 0x61
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64NE : public Instruction { // 0x62
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Add : public Instruction { // 0x6a
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Sub : public Instruction { // 0x6b
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Mul : public Instruction { // 0x6c
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Div_S : public Instruction { // 0x6d
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Div_U : public Instruction { // 0x6e
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Rem_S : public Instruction { // 0x6f
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Rem_U : public Instruction { // 0x70
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32And : public Instruction { // 0x71
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Or : public Instruction { // 0x72
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Xor : public Instruction { // 0x73
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Shl : public Instruction { // 0x74
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Shr_S : public Instruction { // 0x75
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Shr_U : public Instruction { // 0x76
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Add : public Instruction { // 0x7c
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Sub : public Instruction { // 0x7d
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Mul : public Instruction { // 0x7e
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Div_S : public Instruction { // 0x7f
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Div_U : public Instruction { // 0x80
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64And : public Instruction { // 0x83
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Or : public Instruction { // 0x84
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Xor : public Instruction { // 0x85
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Shl : public Instruction { // 0x86
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Shr_S : public Instruction { // 0x87
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Shr_U : public Instruction { // 0x88
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Neg : public Instruction { // 0x9a
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Add : public Instruction { // 0xa0
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Sub: public Instruction { // 0xa1
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Mul: public Instruction { // 0xa2
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Wrap_I64 : public Instruction { // 0xa7
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Trunc_S_F64 : public Instruction { // 0xaa
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I32Trunc_U_F64 : public Instruction { // 0xab
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Extend_S_I32 : public Instruction { // 0xac
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Extend_U_I32 : public Instruction { // 0xad
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Convert_S_I32 : public Instruction { // 0xb7
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Convert_U_I32 : public Instruction { // 0xb8
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class I64Reinterpret_F64 : public Instruction { // 0xbd
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

class F64Reinterpret_I64 : public Instruction { // 0xbf
public:
    virtual uint8_t opcode() const;
    virtual const char* opcodeName() const;
};

#endif // INSTRUCTION_H
