#include "common.h"
#include "import_section.h"

ImportTypeFunction::ImportTypeFunction(uint32_t type)
        : type_(type) {
}

ExternalKind ImportTypeFunction::externalKind() {
    return ExternalKind::Function;
}

ImportTypeTable::ImportTypeTable(ElemType element_type, ResizableLimits limits)
        : type_(element_type, limits) {
}

ExternalKind ImportTypeTable::externalKind() {
    return ExternalKind::Table;
}

ImportTypeMemory::ImportTypeMemory(ResizableLimits limits)
        : type_(limits) {
}

ExternalKind ImportTypeMemory::externalKind() {
    return ExternalKind::Memory;
}

ImportTypeGlobal::ImportTypeGlobal(ValueType content_type, uint8_t mutability)
        : type_(content_type, mutability) {
}

ExternalKind ImportTypeGlobal::externalKind() {
    return ExternalKind::Global;
}

ImportEntry::ImportEntry(uint32_t module_len, uint8_t* module_str, uint32_t field_len, uint8_t* field_str, ImportType* type)
        : module_len_(module_len)
        , module_str_(module_str)
        , field_len_(field_len)
        , field_str_(field_str)
        , type_(type) {
}

uint32_t ImportSection::count() {
    return import_entries_.size();
}

void ImportSection::addImportEntry(const ImportEntry* import_entry) {
    import_entries_.emplace_back(import_entry);
}
