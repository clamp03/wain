#ifndef MEMORY_SECTION_H
#define MEMORY_SECTION_H

#include <vector>

#include "type.h"

class MemorySection {
public:
    uint32_t count();
    void addMemoryType(const MemoryType* memory_type_entry);

private:
    std::vector<const MemoryType*>memory_type_entries_; 
};

#endif // MEMORY_SECTION_H
