#include "common.h"
#include "init.h"

InitExpr::InitExpr() {
}

void InitExpr::addInstruction(Instruction* inst) {
    instructions_.emplace_back(inst);
}

#include "type.h"
