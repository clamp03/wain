#ifndef MODULE_H
#define MODULE_H

#include "common.h"
#include "loader.h"
#include "section.h"

class Module {
public:
    Module();

    bool loadFile(const char* fname);
    bool load(const uint8_t* buf, size_t size);

    bool run();

private:
    bool loadMagicNumber(Loader& l);
    bool loadVersion(Loader& l);

    uint32_t version_;
    Sections* sections_;
};

#endif // MODULE_H
