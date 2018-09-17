#include "instruction.h"

#define DEFINE_OPCODE_FUNCTION(name, code) uint8_t name::opcode() const { return code; }
ENUMERATE_OPCODE(DEFINE_OPCODE_FUNCTION);
#undef DEFINE_OPCODE

Block::Block(int8_t type)
        : type_(type) {
}

Loop::Loop(int8_t type)
        : type_(type) {
}

If::If(int8_t type)
        : type_(type) {
}

End::End() {
}

Br::Br(uint32_t depth)
        : depth_(depth) {
}

BrIf::BrIf(uint32_t depth)
        : depth_(depth) {
}

uint32_t BrTable::count() {
    return targets_.size();
}

void BrTable::addTargetTable(uint32_t target) {
    targets_.emplace_back(target);
}

void BrTable::setDefaultTarget(uint32_t target) {
    default_target_ = target;
}

Call::Call(uint32_t index)
        : index_(index) {
}

CallIndirect::CallIndirect(uint32_t type_index, int8_t reserved)
        : type_index_(type_index)
        , reserved_(reserved) {
}

GetLocal::GetLocal(uint32_t local_index)
        : local_index_(local_index) {
}

SetLocal::SetLocal(uint32_t local_index)
        : local_index_(local_index) {
}

TeeLocal::TeeLocal(uint32_t local_index)
        : local_index_(local_index) {
}

GetGlobal::GetGlobal(uint32_t global_index)
        : global_index_(global_index) {
}

SetGlobal::SetGlobal(uint32_t global_index)
        : global_index_(global_index) {
}

I32Load::I32Load(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load::I64Load(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

F32Load::F32Load(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

F64Load::F64Load(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Load8S::I32Load8S(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Load8U::I32Load8U(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Load16S::I32Load16S(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Load16U::I32Load16U(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load8S::I64Load8S(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load8U::I64Load8U(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load16S::I64Load16S(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load16U::I64Load16U(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load32S::I64Load32S(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Load32U::I64Load32U(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Store::I32Store(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Store::I64Store(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

F32Store::F32Store(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

F64Store::F64Store(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Store8::I32Store8(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I32Store16::I32Store16(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Store8::I64Store8(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Store16::I64Store16(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

I64Store32::I64Store32(uint32_t flags, uint32_t offset)
        : flags_(flags)
        , offset_(offset) {
}

CurrentMemory::CurrentMemory(uint8_t reserved)
        : reserved_(reserved) {
}

GrowMemory::GrowMemory(uint8_t reserved)
        : reserved_(reserved) {
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
