#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdint.h>

class ReaderV1;

class Reader {
public:
    explicit Reader(const char* fname);
    ~Reader();
    bool open();
    void close();
    void read();

private:
    bool readMagicNumber();
    bool readVersion();

    const char* fname_;
    FILE* fp_;

    uint32_t version_;
};

class ReaderV1 {
public:
    explicit ReaderV1(FILE* fp);
    bool readSections();

private:
    uint8_t readVarUint1();
    uint8_t readVarUint7();
    uint32_t readVarUint32();

    int8_t readVarInt7();
    int32_t readVarInt32();
    int64_t readVarInt64();

    const char* readBytes(uint32_t len);

    bool readTypeSection(uint32_t len, const char* data);

    enum class SectionId {
        NAME = 0,
        TYPE,
        IMPORT,
        FUNCTION,
        TABLE,
        MEMORY,
        GLOBAL,
        EXPORT,
        START,
        ELEMENT,
        CODE,
        DATA
    };

    FILE* fp_;
};

#endif
