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

uint32_t FunctionBody::localCount() {
    return locals_.size();
}

uint32_t CodeSection::count() {
    return bodies_.size();
}

void CodeSection::addFunctionBody(FunctionBody* body) {
    bodies_.emplace_back(body);
}

const FunctionBody* CodeSection::getFunctionBody(uint32_t idx)
{
    DEV_ASSERT(idx < count(), "Indvalid Function Body Index");
    return bodies_.at(idx);
}
