#include "common.h"
#include "table_section.h"

uint32_t TableSection::count() {
    return table_type_entries_.size();
}

void TableSection::addTableType(const TableType* table_type_entry) {
    table_type_entries_.emplace_back(table_type_entry);
}
