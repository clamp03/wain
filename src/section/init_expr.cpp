#include "common.h"
#include "init_expr.h"

InitExpr::InitExpr() {
}

void InitExpr::addInstruction(Instruction* inst) {
    instructions_.emplace_back(inst);
}
