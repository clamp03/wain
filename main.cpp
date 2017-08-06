#include "reader.h"

int main(int argc, char* argv[]) {
    if (argc == 0) return -1;

    // TODO: Configuration from command arguments
    // char* fname = argv[1];
    const char* fname = "hell.wasm";

    Reader r(fname);
    r.read();
    return 0;
}
