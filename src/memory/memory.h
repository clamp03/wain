#ifndef MEM_H
#define MEM_H

#include "common.h"

struct BufNode {
    BufNode* next;
    size_t size;
    size_t offset;
    uint8_t* buf;
};

class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();
    void* allocate(size_t size);
    void deallocateAll();

private:
    BufNode* head_;
    BufNode* curr_;
};

#endif // MEM_H
