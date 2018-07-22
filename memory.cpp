#include "memory.h"

MemoryManager::MemoryManager() {
    head_ = static_cast<BufNode*>(malloc(sizeof(BufNode)));
    tail_ = static_cast<BufNode*>(malloc(sizeof(BufNode)));
    head_->prev = nullptr;
    head_->next = tail_;
    head_->buf = nullptr;
    tail_->prev = head_;
    tail_->next = nullptr;
    tail_->buf = nullptr;
}

MemoryManager::~MemoryManager() {
    BufNode* curr = head_->next;
    while (curr != tail_) {
        BufNode* cand = curr;
        curr = curr->next;
        deallocate(cand);
    }
    free(head_);
    free(tail_);
}

void* MemoryManager::allocate(size_t size) {
    BufNode* curr = static_cast<BufNode*>(malloc(sizeof(BufNode)));
    BufNode* last = tail_->prev;

    last->next = curr;
    tail_->prev = curr;
    curr->prev = last;
    curr->next = tail_;
    curr->buf = static_cast<uint8_t*>(malloc(size));
    return curr->buf;
}

void MemoryManager::deallocate(BufNode* buf) {
    BufNode* prev = buf->prev;
    BufNode* next = buf->next;
    prev->next = next;
    next->prev = prev;
    free(buf->buf);
    free(buf);
}
