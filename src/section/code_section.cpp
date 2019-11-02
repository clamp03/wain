#include "code_section.h"

Local::Local(uint32_t count, int8_t type)
        : count_(count)
        , type_(type) {
}

void FunctionBody::addLocal(Local* local) {
    locals_.emplace_back(local);
}

void FunctionBody::addInstruction(Instruction* inst) {
    instructions_.emplace_back(inst);
}

uint32_t FunctionBody::localCount() const {
    return locals_.size();
}

uint32_t FunctionBody::instructionCount() const {
    return instructions_.size();
}

const Local* FunctionBody::getLocal(uint32_t idx) const {
    DEV_ASSERT(idx < localCount(), "Invalid Function Local Index");
    return locals_.at(idx);
}

const Instruction* FunctionBody::getInstruction(uint32_t idx) const {
    DEV_ASSERT(idx < instructionCount(), "Invalid Function Instruction Index");
    return instructions_.at(idx);
}

uint32_t CodeSection::count() const {
    return bodies_.size();
}

void CodeSection::addFunctionBody(FunctionBody* body) {
    bodies_.emplace_back(body);
}

const FunctionBody* CodeSection::getFunctionBody(uint32_t idx) const {
    DEV_ASSERT(idx < count(), "Indvalid Function Body Index");
    return bodies_.at(idx);
}
