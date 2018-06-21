#ifndef LOADER_H
#define LOADER_H

#include "common.h"


class Loader {
public:
    Loader(const uint8_t* buf, size_t size);

    uint8_t loadVarUint1();
    uint8_t loadVarUint7();
    uint32_t loadVarUint32();

    int8_t loadVarInt7();
    int32_t loadVarInt32();
    int64_t loadVarInt64();

    uint32_t loadUint32();
    uint64_t loadUint64();

    uint8_t loadOpcode();
    void loadBytes(void* buf, size_t len);

    size_t index();
    size_t size();

private:
    const uint8_t* buf_;
    size_t idx_;
    size_t size_;
};
#endif // LOADER_H
