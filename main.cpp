#include "reader.h"

int main(int argc, char* argv[]) {
    if (argc == 0) return -1;

    char* fname = argv[1];

    Reader r(fname);
    if (r.open()) {
        r.read();
        r.close();
    }
    return 0;
}
