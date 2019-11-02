#include "interpreter.h"

Interpreter::Interpreter(const Sections* sections)
        : sections_(sections) {
    stack_ = new FrameStack();
}

Interpreter::~Interpreter() {
    delete stack_;
}

int32_t Interpreter::start(uint32_t idx) {
    int32_t val = 0;
    pushFrame(idx);
    run(idx, true);
    const FuncType* type = currentFuncType();
    if (type->hasReturnType()) {
        DEV_ASSERT(type->returnType() == I32, "FIXME: Not Supporing Return Type");
        DEV_ASSERT(operandSize() == 1, "Operand Size Should be 1");
        val = popOperand();
    } else {
        DEV_ASSERT(operandSize() == 0, "Operand Size Should be 0");
    }
    popFrame();
    return val;
}

void Interpreter::pushFrame(uint32_t idx) {
    const FunctionBody* body = sections_->getCodeSection()->getFunctionBody(idx);
    const FuncType* type = sections_->getTypeSection()->funcType(sections_->getFunctionSection()->type(idx));
    stack_->pushFrame(body, type, idx);
}

void Interpreter::popFrame() {
    DEV_ASSERT(currentFrame(), "CurrentFrame should not be NULL");
    stack_->popFrame();
}

Frame* Interpreter::currentFrame() const {
    DEV_ASSERT(stack_->currentFrame(), "CurrentFrame should not be NULL");
    return stack_->currentFrame();
}

const FunctionBody* Interpreter::currentFunctionBody() const {
    return currentFrame()->functionBody();
}

const FuncType* Interpreter::currentFuncType() const {
    return currentFrame()->funcType();
}

void Interpreter::pushOperand(int32_t operand) {
    currentFrame()->pushOperand(operand);
}

int32_t Interpreter::popOperand() {
    return currentFrame()->popOperand();
}

uint32_t Interpreter::operandSize() {
    return currentFrame()->operandSize();
}

int32_t Interpreter::local(uint32_t idx) {
    return currentFrame()->local(idx);
}

void Interpreter::run(uint32_t idx, bool skip) {
    uint8_t opcode;
    uint32_t block = 1;

    if (!skip) pushFrame(idx);
    const FunctionBody* body = currentFunctionBody();
    for (int i = 0; i < body->instructionCount(); i++) {
        const Instruction* inst = body->getInstruction(i);
        opcode = inst->opcode();
        switch (inst->opcode()) {
        case EndOpcode:
            block--;
            break;
        case CallOpcode:
            run(((Call*)inst)->index());
            break;
        case GetLocalOpcode: {
            uint32_t idx = ((GetLocal*)inst)->index();
            int32_t val = local(idx);
            pushOperand(val);
            break;
        }
        case I32ConstOpcode:
            pushOperand(((I32Const*)inst)->value());
            break;
        case I32ShlOpcode:
        {
            int32_t val1 = popOperand();
            int32_t val2 = popOperand();
            int32_t val = val2 << val1;
            pushOperand(val);
            break;
        }
        default:
            NOT_YET_IMPLEMENTED
        }
    }
    DEV_ASSERT(block == 0 && opcode == 0xb, "Invalid Termination of Function Body");
    if (!skip) popFrame();
}
