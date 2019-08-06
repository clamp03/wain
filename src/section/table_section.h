#ifndef TABLE_SECTION_H
#define TABLE_SECTION_H

#include <vector>

#include "type.h"

class TableSection {
public:
    uint32_t count();
    void addTableType(const TableType* table_type_entry);

private:
    std::vector<const TableType*>table_type_entries_; 
};

#endif // TABLE_SECTION_H
