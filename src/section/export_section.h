#ifndef EXPORT_SECTION_H 
#define EXPORT_SECTION_H

#include <vector>

#include "type.h"

class ExportEntry {
public:
    ExportEntry(uint32_t field_len, uint8_t* field_str, ExternalKind kind, uint32_t index);

    uint32_t getFieldLen() const;
    const uint8_t* getFieldStr() const;
    ExternalKind getKind() const;
    uint32_t getIndex() const;

private:
    uint32_t field_len_;
    uint8_t* field_str_;
    ExternalKind kind_;
    uint32_t index_;
};

class ExportSection {
public:
    uint32_t count();
    void addExportEntry(const ExportEntry* export_entry);
    const ExportEntry* getExportEntry(uint32_t idx);

private:
    std::vector<const ExportEntry*> export_entries_;
};

#endif // EXPORT_SECTION_H
