#include <assert.h>
#include <string.h>

#include "reader.h"

Reader::Reader(const char* fname)
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
    if (version_ == 1) {
        ReaderV1 v1(fp_);
        assert(v1.readSections());
    }
}

bool Reader::readMagicNumber() {
#define MAGIC_NUMBER "\0asm"
#define MAGIC_NUMBER_SIZE sizeof(uint32_t)
    char magic_number[MAGIC_NUMBER_SIZE];
    fread(magic_number, 1, MAGIC_NUMBER_SIZE, fp_);
    return strncmp(magic_number, MAGIC_NUMBER, MAGIC_NUMBER_SIZE) == 0;
}

bool Reader::readVersion() {
#define VERSION_SIZE sizeof(uint32_t)
    return fread(&version_, 1, VERSION_SIZE, fp_) == VERSION_SIZE;
}

ReaderV1::ReaderV1(FILE* fp)
        : fp_(fp) {}

bool ReaderV1::readSections() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return true;
}

uint8_t ReaderV1::readVarUint1() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return 0;
}

uint8_t ReaderV1::readVarUint7() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return 0;
}

uint32_t ReaderV1::readVarUint32() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return 0;
}

int8_t ReaderV1::readVarInt7() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return 0;
}

int32_t ReaderV1::readVarInt32() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return 0;
}

int64_t ReaderV1::readVarInt64() {
    assert(false); // TODO: NOT YET IMPLEMENTED
    return 0;
}
