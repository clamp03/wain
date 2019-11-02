#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "common.h"
#include "section.h"
#include "stack.h"

class Interpreter {
public:
    Interpreter(const Sections* sections);
    virtual ~Interpreter();

    int32_t start(uint32_t idx);

private:
    void pushFrame(uint32_t idx);
    void popFrame();
    Frame* currentFrame() const;
    const FunctionBody* currentFunctionBody() const;
    const FuncType* currentFuncType() const;
    void pushOperand(int32_t operand);
    int32_t popOperand();
    uint32_t operandSize();
    int32_t local(uint32_t idx);
    void run(uint32_t idx, bool skip = false);
    const Sections* sections_;
    FrameStack* stack_;
};

#endif // INTERPRETER_H
