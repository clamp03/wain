#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "common.h"
#include "section_v1.h"

class Interpreter {
public:
    Interpreter();
    virtual ~Interpreter();

    static void run(const FunctionBody* body);

private:
};

#endif // INTERPRETER_H
