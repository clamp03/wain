#include "common.h"
#include "module.h"

class Module;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid usage" << std::endl;
        std::cerr << "./wasmIntrp <file name>" << std::endl;
        return -1;
    }

    char* fname = argv[1];

    Module m;
    if (m.loadFile(fname)) {
        m.run();
    }
    return 0;
}
