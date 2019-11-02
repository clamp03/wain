#ifndef STACK_H
#define STACK_H

#include "common.h"
#include "section.h"

class Frame {
public:
    Frame(const FunctionBody* body, const FuncType* type, uint32_t idx, Frame* caller);
    virtual ~Frame();
    Frame* callerFrame() const;

    const FunctionBody* functionBody() const;
    const FuncType* funcType() const;

    void pushOperand(int32_t operand); // FIXME: Support other types
    int32_t popOperand(); // FIXME: Support other types;
    uint32_t operandSize();

    void copyParam(int32_t local); // FIXME: Support other types
    int32_t local(uint32_t idx); // FIXME: Support other types

private:
    const FunctionBody* body_;
    const FuncType* type_;
    uint32_t idx_;
    Frame* caller_;
    std::vector<int32_t> locals_; // FIXME: Support other types
    std::vector<int32_t> operands_; // FIXME: Support other types
};

class FrameStack {
public:
    FrameStack();
    virtual ~FrameStack();
    void pushFrame(const FunctionBody* body, const FuncType* type, uint32_t idx);
    void popFrame();

    Frame* currentFrame();

private:
    Frame* frame_;
};

#endif // STACK_H
