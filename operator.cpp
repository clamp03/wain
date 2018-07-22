#include "operator.h"

#define ENUMERATE_OPCODE(FUNC) \
    FUNC(End, 0x0b)            \
    FUNC(GetGlobal, 0x23)      \
    FUNC(I32Const, 0x41)       \
    FUNC(I64Const, 0x42)       \
    FUNC(F32Const, 0x43)       \
    FUNC(F64Const, 0x44)


#define DEFINE_OPCODE(name, code) uint8_t name##Operator::opcode() const { return code; }
ENUMERATE_OPCODE(DEFINE_OPCODE);
#undef DEFINE_OPCODE

EndOperator::EndOperator() {
}

GetGlobalOperator::GetGlobalOperator(uint32_t global_index)
        : global_index_(global_index) {
}

I32ConstOperator::I32ConstOperator(int32_t value)
       : value_(value) {
}

I64ConstOperator::I64ConstOperator(int64_t value)
       : value_(value) {
}

F32ConstOperator::F32ConstOperator(uint32_t value)
       : value_(value) {
}

F64ConstOperator::F64ConstOperator(uint64_t value)
       : value_(value) {
}
