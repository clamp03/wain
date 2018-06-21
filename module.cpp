#include <iostream>
#include <stdio.h>
#include <string.h>

#include "module.h"
#include "section.h"

Module::Module()
    : version_(0),
      sections_(nullptr) {
}


bool Module::loadFile(const char* fname) {
    FILE* fp = fopen(fname, "rb");
    uint8_t* buf = nullptr;
    if (!fp) {
        cerr << "Cannot open file: " << fname << endl;
        return false;
    }

    fseek(fp, 0 , SEEK_END);
    size_t size = ftell(fp);

    buf = (uint8_t*)malloc(sizeof(uint8_t) * size);
    rewind(fp);
    fread(buf, 1, size, fp);
    load(buf, size);

    free(buf);
    fclose(fp);
    return true;
}

bool Module::load(const uint8_t* buf, size_t size) {
    if (buf == nullptr) {
        return false;
    }
    Loader l(buf, size);
    DEV_ASSERT(loadMagicNumber(l), "Invalid magic number");
    DEV_ASSERT(loadVersion(l), "Invalid version number");

    DEV_ASSERT(version_ == 1, "Not supported version number");
    if (version_ == 1) {
        sections_ = new SectionsV1();
        sections_->load(l);
    } else {
        // Currently, there is only version 1
        return false;
    }
    return true;
}

bool Module::loadMagicNumber(Loader& r) {
#define MAGIC_NUMBER "\0asm"
#define MAGIC_NUMBER_SIZE sizeof(uint32_t)
    char magic_number[MAGIC_NUMBER_SIZE];
    r.loadBytes(magic_number, MAGIC_NUMBER_SIZE);
    return strncmp(magic_number, MAGIC_NUMBER, MAGIC_NUMBER_SIZE) == 0;
}

bool Module::loadVersion(Loader& r) {
    version_ = r.loadUint32();
    return true;
}


bool Module::run() {
    // TODO
    return true;
}
