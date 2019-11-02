#include "stack.h"

Frame::Frame(const FunctionBody* body, const FuncType* type, uint32_t idx, Frame* caller)
        : body_(body)
        , type_(type)
        , idx_(idx)
        , caller_(caller) {
}

Frame::~Frame() {
}

Frame* Frame::callerFrame() const {
    return caller_;
}


const FunctionBody* Frame::functionBody() const {
    return body_;
}

const FuncType* Frame::funcType() const {
    return type_;
}

void Frame::pushOperand(int32_t operand) {
    operands_.push_back(operand);
}

int32_t Frame::popOperand() {
    DEV_ASSERT(operands_.size() != 0, "Operand Stack is Empty");
    int32_t operand = operands_.back();
    operands_.pop_back();
    return operand;
}

uint32_t Frame::operandSize() {
    return operands_.size();
}

void Frame::copyParam(int32_t local) {
    locals_.insert(locals_.begin(), local);
}

int32_t Frame::local(uint32_t idx) {
    DEV_ASSERT(idx < locals_.size(), "Invalid Local Index");
    return locals_.at(idx);
}

FrameStack::FrameStack() {
    frame_ = nullptr;
}

FrameStack::~FrameStack() {
    while(frame_) {
        popFrame();
    }
}

void FrameStack::pushFrame(const FunctionBody* body, const FuncType* type, uint32_t idx) {
    Frame* f = new Frame(body, type, idx, frame_);
    for (int i = 0; i < type->paramCount(); i++) {
        f->copyParam(frame_->popOperand());
    }
    frame_ = f;
}

void FrameStack::popFrame() {
    if (frame_ != nullptr) {
        Frame* caller = frame_->callerFrame();
        const FuncType* type = frame_->funcType();
        if (caller != nullptr && type->hasReturnType()) {
            DEV_ASSERT(type->returnType() == I32, "FIXME: Not Supporing Return Type");
            int32_t val = frame_->popOperand();
            caller->pushOperand(val);
        }
        delete frame_;
        frame_ = caller;
    }
}

Frame* FrameStack::currentFrame() {
    return frame_;
}
