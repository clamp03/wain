#ifndef INIT_H
#define INIT_H

#include <vector>

#include "instruction.h"

class InitExpr {
public:
    InitExpr();

    void addInstruction(Instruction* inst);

private:
    std::vector<Instruction*> instructions_;
};

#endif // INIT_H
