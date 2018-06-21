#include <iostream>

#include "module.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Invalid usage" << endl;
        cerr << "./wasmIntrp <file name>" << endl;
        return -1;
    }

    char* fname = argv[1];

    Module m;
    if (m.loadFile(fname)) {
        m.run();
    }
    return 0;
}
