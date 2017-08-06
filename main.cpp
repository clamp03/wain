#include "reader.h"

int main(int argc, char* argv[]) {
    if (argc == 0) return -1;

    // TODO: Configuration from command arguments
    char* fname = argv[1];

    Reader r(fname);
    r.read();
    return 0;
}
