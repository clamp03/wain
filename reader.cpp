#include <assert.h>
#include <string.h>

#include <iostream>

#include "reader.h"
#include "common.h"

using namespace std;

Reader::Reader(const char* fname)
        : fname_(fname),
          fp_(nullptr),
          version_(0) {}

Reader::~Reader() {
    close();
}

bool Reader::open() {
    fp_ = fopen(fname_, "rb");
    if (fp_) return true;
    cerr << "Cannot open file: " << fname_ << endl;
    return false;
}

void Reader::close() {
    fclose(fp_);
    fp_ = nullptr;
}

void Reader::read() {
    DEV_ASSERT(readMagicNumber(), "Invalid magic number");
    DEV_ASSERT(readVersion(), "Invalid version number");
    if (version_ == 1) {
        ReaderV1 v1(fp_);
        DEV_ASSERT(v1.readSections(), "Invalid sections");
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
    while (true) {
        uint8_t id = readVarUint7();
        uint32_t payload_len = readVarUint32();
        uint32_t name_len = 0;
        const char* name = nullptr;
        if (id == 0) {
            name_len = readVarUint32();
            name = readBytes(name_len);
            payload_len -= (name_len + 4); // FIXME: correct length of name and name_len
        }
        const char* payload_data = readBytes(payload_len);

        switch (static_cast<SectionId>(id)) {
            case SectionId::NAME:
                cerr << "Section Id: " << static_cast<int>(id) << endl;
                NOT_YET_IMPLEMENTED
            case SectionId::TYPE:
                readTypeSection(payload_len, payload_data);
                break;
            case SectionId::IMPORT:
            case SectionId::FUNCTION:
            case SectionId::TABLE:
            case SectionId::MEMORY:
            case SectionId::GLOBAL:
            case SectionId::EXPORT:
            case SectionId::START:
            case SectionId::ELEMENT:
            case SectionId::CODE:
            case SectionId::DATA:
                cerr << "Section Id: " << static_cast<int>(id) << endl;
                NOT_YET_IMPLEMENTED
        }
    }
    return true;
}

uint8_t ReaderV1::readVarUint1() {
    NOT_YET_IMPLEMENTED
}

uint8_t ReaderV1::readVarUint7() {
    uint8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val;
}

uint32_t ReaderV1::readVarUint32() {
    uint32_t val = 0;
    uint32_t shift = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t byte = 0;
        fread(&byte, 1, 1, fp_);
        val |= (byte & 0x7f) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7; 
    }
    return val;
}

int8_t ReaderV1::readVarInt7() {
    NOT_YET_IMPLEMENTED
}

int32_t ReaderV1::readVarInt32() {
    NOT_YET_IMPLEMENTED
}

int64_t ReaderV1::readVarInt64() {
    NOT_YET_IMPLEMENTED
}

const char* ReaderV1::readBytes(uint32_t len) {
    char* val = static_cast<char*>(malloc(len));
    fread(val, 1, len, fp_);
    return val;
}

bool ReaderV1::readTypeSection(uint32_t len, const char* data) {
    NOT_YET_IMPLEMENTED
}
