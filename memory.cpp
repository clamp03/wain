#include <iostream>

#include "common.h"
#include "memory.h"

MemoryManager::MemoryManager() {
    head_ = static_cast<BufNode*>(malloc(sizeof(BufNode)));
    tail_ = static_cast<BufNode*>(malloc(sizeof(BufNode)));
    head_->prev = nullptr;
    head_->next = tail_;
    tail_->prev = head_;
    tail_->next = nullptr;
}

MemoryManager::~MemoryManager() {
    BufNode* curr = head_;
    while (curr != nullptr) {
        BufNode* cand = curr;
        curr = curr->next;
        free(cand);
    }
}

void* MemoryManager::allocate(size_t size) {
    BufNode* curr = static_cast<BufNode*>(malloc(sizeof(BufNode)));
    BufNode* last = tail_->prev;

    last->next = curr;
    curr->prev = last;
    curr->next = tail_;
    return curr;
}

void MemoryManager::deallocate(BufNode* buf) {
    BufNode* prev = buf->prev;
    BufNode* next = buf->next;
    prev->next = next;
    next->prev = prev;
    free (buf);
}
