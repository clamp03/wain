#include "common.h"
#include "memory_section.h"

uint32_t MemorySection::count() {
    return memory_type_entries_.size();
}

void MemorySection::addMemoryType(const MemoryType* memory_type_entry) {
    memory_type_entries_.emplace_back(memory_type_entry);
}
