#include <assert.h>
#include <string.h>

#include "reader.h"

Reader::Reader(char* fname)
        : fname_(fname),
          fp_(nullptr),
          version_(0) {
    fp_ = fopen(fname, "rb");
}

Reader::~Reader() {
    fclose(fp_);
}

void Reader::read() {
    assert(readMagicNumber());
    assert(readVersion());
}

bool Reader::readMagicNumber() {
#define MAGIC_NUMBER "\0asm"
#define MAGIC_NUMBER_SIZE sizeof(uint32_t)
    char magic_number[MAGIC_NUMBER_SIZE];
    fread(magic_number, MAGIC_NUMBER_SIZE, 1, fp_);
    return strncmp(magic_number, MAGIC_NUMBER, MAGIC_NUMBER_SIZE) == 0;
}

bool Reader::readVersion() {
#define VERSION_SIZE sizeof(uint32_t)
    return fread(&version_, VERSION_SIZE, 1, fp_) == 1;
}
