#ifndef MEM_H
#define MEM_H

#include "common.h"

struct BufNode {
    BufNode* prev;
    BufNode* next;
    uint8_t* buf;
};

class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();
    void* allocate(size_t size);
    void deallocate(BufNode* buf);

private:
    BufNode* head_;
    BufNode* tail_;
};

#endif // MEM_H
