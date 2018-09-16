#include "instruction.h"

#define DEFINE_OPCODE_FUNCTION(name, code) uint8_t name::opcode() const { return code; }
ENUMERATE_OPCODE(DEFINE_OPCODE_FUNCTION);
#undef DEFINE_OPCODE

End::End() {
}

GetGlobal::GetGlobal(uint32_t global_index)
        : global_index_(global_index) {
}

I32Const::I32Const(int32_t value)
       : value_(value) {
}

I64Const::I64Const(int64_t value)
       : value_(value) {
}

F32Const::F32Const(uint32_t value)
       : value_(value) {
}

F64Const::F64Const(uint64_t value)
       : value_(value) {
}
