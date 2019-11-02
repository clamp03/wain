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

    uint32_t localCount() const;
    uint32_t instructionCount() const;

    const Local* getLocal(uint32_t idx) const;
    const Instruction* getInstruction(uint32_t idx) const;

private:
    std::vector<Local*> locals_;
    std::vector<Instruction*> instructions_;
};


class CodeSection {
public:
    uint32_t count() const;
    void addFunctionBody(FunctionBody* body);
    const FunctionBody* getFunctionBody(uint32_t idx) const;

private:
    std::vector<FunctionBody*> bodies_;
    
};

#endif // CODE_SECTION_H
