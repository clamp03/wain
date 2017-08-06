#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdint.h>

class Reader {
public:
    explicit Reader(char* fname);
    ~Reader();
    void read();

private:
    bool readMagicNumber();
    bool readVersion();

    char* fname_;
    FILE* fp_;

    uint32_t version_;
};

#endif
