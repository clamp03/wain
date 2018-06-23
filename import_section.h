#ifndef IMPORT_SECTION_H 
#define IMPORT_SECTION_H

#include <vector>

#include "common.h"
#include "section.h"
#include "type.h"

class ImportType {
public:
    virtual ExternalKind externalKind() = 0;
};

class ImportTypeFunction : public ImportType {
public:
    virtual ExternalKind externalKind();

private:
    uint32_t type_;
};

class ImportTypeTable : public ImportType {
public:
    virtual ExternalKind externalKind();

private:
    TableType type_;
};

class ImportTypeMemory: public ImportType {
public:
    virtual ExternalKind externalKind();

private:
    MemoryType type_;
};

class ImportTypeGlobal : public ImportType {
public:
    virtual ExternalKind externalKind();

private:
    GlobalType type_;
};

class ImportEntry {
public:

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
