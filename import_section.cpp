#include "import_section.h"

ExternalKind ImportTypeFunction::externalKind() {
    return ExternalKind::Function;
}

ExternalKind ImportTypeTable::externalKind() {
    return ExternalKind::Table;
}

ExternalKind ImportTypeMemory::externalKind() {
    return ExternalKind::Memory;
}

ExternalKind ImportTypeGlobal::externalKind() {
    return ExternalKind::Global;
}

uint32_t ImportSection::count() {
    return import_entries_.size();
}

void ImportSection::addImportEntry(const ImportEntry* import_entry) {
    import_entries_.emplace_back(import_entry);
}
