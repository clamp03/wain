#include "common.h"
#include "export_section.h"

ExportEntry::ExportEntry(uint32_t field_len, uint8_t* field_str, ExternalKind kind, uint32_t index)
        : field_len_(field_len)
        , field_str_(field_str)
        , kind_(kind)
        , index_(index) {
}

uint32_t ExportEntry::getFieldLen() const {
    return field_len_;
}

const uint8_t* ExportEntry::getFieldStr() const {
    return field_str_;
}

ExternalKind ExportEntry::getKind() const {
    return kind_;
}

uint32_t ExportEntry::getIndex() const {
    return index_;
}

uint32_t ExportSection::count() {
    return export_entries_.size();
}

void ExportSection::addExportEntry(const ExportEntry* export_entry) {
    export_entries_.emplace_back(export_entry);
}

const ExportEntry* ExportSection::getExportEntry(uint32_t idx) {
    DEV_ASSERT(idx < count(), "Invalid Export Entry Index");
    return export_entries_.at(idx);
}
