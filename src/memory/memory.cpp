#include "memory.h"

#define DEFAULT_BLOCK_SIZE 4 * 1024

MemoryManager::MemoryManager() {
    head_ = curr_ = nullptr;
}

MemoryManager::~MemoryManager() {
    deallocateAll();
}

void* MemoryManager::allocate(size_t size) {
    if (curr_ == nullptr || curr_->size - curr_->offset < size) {
        BufNode* next = static_cast<BufNode*>(malloc(sizeof(BufNode)));
        size_t block_size = size > DEFAULT_BLOCK_SIZE ? size : DEFAULT_BLOCK_SIZE;
        next->next = nullptr;
        next->size = block_size;
        next->offset = size;
        next->buf = static_cast<uint8_t*>(malloc(block_size));
        if (head_ == nullptr) {
            head_ = curr_ = next;
        } else {
            curr_->next = next;
            curr_ = next;
        }
        return next->buf;
    }

    uint8_t* ret = curr_->buf + curr_->offset;
    curr_->offset += size;
    return static_cast<void*>(ret);
}

void MemoryManager::deallocateAll() {
    while(head_ != nullptr) {
        BufNode* cand = head_;
        head_ = head_->next;
        free(cand->buf);
        free(cand);
    }
}
