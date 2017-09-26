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

        cerr << "Section: " << (int)id << " " << (uint)payload_len << endl;

        // TODO: check type index is increasing
        switch (static_cast<SectionId>(id)) {
            case SectionId::NAME:
                NOT_YET_IMPLEMENTED
            case SectionId::TYPE:
                if (!readTypeSection(payload_len)) {
                    DEV_ASSERT(false, "INVALID PAYLOAD");
                }
                break;
            case SectionId::IMPORT:
                if (!readImportSection(payload_len)) {
                    DEV_ASSERT(false, "INVALID PAYLOAD");
                }
                break;
            case SectionId::FUNCTION:
            case SectionId::TABLE:
            case SectionId::MEMORY:
            case SectionId::GLOBAL:
            case SectionId::EXPORT:
            case SectionId::START:
            case SectionId::ELEMENT:
            case SectionId::CODE:
            case SectionId::DATA:
            default:
                cerr << "Section Id: " << static_cast<int>(id) << endl;
                NOT_YET_IMPLEMENTED
        }
    }
    return true;
}

bool ReaderV1::readTypeSection(uint32_t payload_len) {
    uint32_t type_count = readVarUint32();
    uint32_t start = ftell(fp_);
    cerr << "TYPE COUNT: " << type_count << endl;
    for (uint32_t type = 0; type < type_count; type++) {
        int8_t form = readVarInt7();
        uint32_t param_count = readVarUint32();
        cerr << "FORM: " << (int)form << " " << param_count << endl;
        for (uint32_t param = 0; param < param_count; param++) {
            int8_t value_type = readVarInt7();
            // TODO
            cerr << "VALUE TYPE: " << (int)value_type << endl;
        }
        uint8_t return_count = readVarUint1();
        if (return_count) {
            int8_t return_type = readVarInt7();
            // TODO
            cerr << "RETURN TYPE: " << (int)return_type << endl;
        }
    }
    uint32_t end = ftell(fp_);
    return ((end - start + 1) == payload_len);
}

bool ReaderV1::readImportSection(uint32_t payload_len) {
    uint32_t import_count = readVarUint32();
    uint32_t start = ftell(fp_);
    cerr << "IMPORT COUNT: " << import_count << endl;
}

uint8_t ReaderV1::readVarUint1() {
    uint8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val & 0x1;
}

uint8_t ReaderV1::readVarUint7() {
    uint8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val & 0x7f;
}

uint32_t ReaderV1::readVarUint32() {
    uint32_t val = 0;
    uint32_t shift = 0;
    for (int i = 0; i < 5; i++) {
        uint8_t byte = 0;
        fread(&byte, 1, 1, fp_);
        val |= (byte & 0x7f) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7; 
    }
    return val;
}

int8_t ReaderV1::readVarInt7() {
    int8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val & 0x7f;
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


