#include <string.h>

#include "common.h"
#include "loader.h"
#include "module.h"

Loader::Loader(const uint8_t* buf, size_t size)
        : buf_(buf),
          idx_(0),
          size_(size) {
}

uint8_t Loader::loadVarUint1() {
    return buf_[idx_++] & 0x1;
}

uint8_t Loader::loadVarUint7() {
    return buf_[idx_++] & 0x7f;
}

uint32_t Loader::loadVarUint32() {
    uint32_t val = 0;
    uint32_t shift = 0;
    for (int i = 0; i < 5; i++) {
        uint8_t byte = buf_[idx_++];
        val |= (byte & 0x7f) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7;
    }
    return val;
}

int8_t Loader::loadVarInt7() {
    int8_t val = static_cast<int8_t>(buf_[idx_++]);
    return val & 0x7f;
}

int32_t Loader::loadVarInt32() {
    int32_t val = 0;
    uint32_t shift = 0;
    uint32_t size = 32;
    uint8_t byte = 0;
    do {
        byte = buf_[idx_++];
        val |= (byte & 0x7) << shift;
        shift += 7;
    } while (byte & 0x80);

    if ((shift < size) && (byte & 0x40)) {
        val |= (~0 << shift);
    }
    return val;
}

int64_t Loader::loadVarInt64() {
    int64_t val = 0;
    uint32_t shift = 0;
    uint32_t size = 64;
    uint8_t byte = 0;
    do {
        byte = buf_[idx_++];
        val |= (byte & 0x7) << shift;
        shift += 7;
    } while (byte & 0x80);

    if ((shift < size) && (byte & 0x40)) {
        val |= (~0 << shift);
    }
    return val;
}

uint32_t Loader::loadUint32() {
    uint32_t val = 0;
    memcpy(&val, &buf_[idx_], 4);
    idx_ += 4;
    return val;
}

uint64_t Loader::loadUint64() {
    uint64_t val = 0;
    memcpy(&val, &buf_[idx_], 8);
    idx_ += 8;
    return val;
}

uint8_t Loader::loadOpcode() {
    return static_cast<int8_t>(buf_[idx_++]);
}

void Loader::loadBytes(void* buf, size_t len) {
    memcpy(buf, &buf_[idx_], len);
    idx_ += len;
}

size_t Loader::index() {
    return idx_;
}

size_t Loader::size() {
    return size_;
}
