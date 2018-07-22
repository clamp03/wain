#ifndef IMPORT_SECTION_H 
#define IMPORT_SECTION_H

#include <vector>

#include "type.h"

class ImportType {
public:
    virtual ExternalKind externalKind() = 0;
};

class ImportTypeFunction : public ImportType {
public:
    ImportTypeFunction(uint32_t type);
    virtual ExternalKind externalKind();

private:
    uint32_t type_;
};

class ImportTypeTable : public ImportType {
public:
    ImportTypeTable(ElemType element_type, ResizableLimits limits);
    virtual ExternalKind externalKind();

private:
    TableType type_;
};

class ImportTypeMemory: public ImportType {
public:
    ImportTypeMemory(ResizableLimits limits);
    virtual ExternalKind externalKind();

private:
    MemoryType type_;
};

class ImportTypeGlobal : public ImportType {
public:
    ImportTypeGlobal(ValueType content_type, uint8_t mutability);
    virtual ExternalKind externalKind();

private:
    GlobalType type_;
};

class ImportEntry {
public:
    ImportEntry(uint32_t module_len, uint8_t* module_str, uint32_t field_len, uint8_t* field_str, ImportType* type);

private:
    uint32_t module_len_;
    uint8_t* module_str_;
    uint32_t field_len_;
    uint8_t* field_str_;
    ImportType* type_;
};

class ImportSection {
public:
    uint32_t count();
    void addImportEntry(const ImportEntry* import_entry);

private:
    std::vector<const ImportEntry*> import_entries_;
};

#endif // IMPORT_SECTION_H
