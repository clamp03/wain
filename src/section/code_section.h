#ifndef CODE_SECTION_H
#define CODE_SECTION_H

#include <vector>

#include "instruction.h"

class Local {
public:
    Local(uint32_t count, int8_t type);

private:
    uint32_t count_;
    int8_t type_;
};

class FunctionBody {
public:
    void addLocal(Local* local);
    void addInstruction(Instruction* inst);

    uint32_t localCount();

private:
    std::vector<Local*> locals_;
    std::vector<Instruction*> instructions_;
};


class CodeSection {
public:
    uint32_t count();
    void addFunctionBody(FunctionBody* body);

private:
    std::vector<FunctionBody*> bodies_;
    
};

#endif // CODE_SECTION_H
