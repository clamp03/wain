#ifndef SECTION_H 
#define SECTION_H

#include "type_section.h"
#include "import_section.h"
#include "function_section.h"
#include "table_section.h"
#include "memory_section.h"
#include "global_section.h"
#include "export_section.h"
#include "element_section.h"
#include "code_section.h"
#include "data_section.h"

enum class SectionId {
    NAME = 0,
    TYPE,
    IMPORT,
    FUNCTION,
    TABLE,
    MEMORY,
    GLOBAL,
    EXPORT,
    START,
    ELEMENT,
    CODE,
    DATA
};

class Sections {
public:
    Sections();
    virtual ~Sections();
    virtual bool load() = 0;
    virtual bool loadTypeSection() = 0;
    virtual bool loadImportSection() = 0;
    virtual bool loadFunctionSection() = 0;
    virtual bool loadTableSection() = 0;
    virtual bool loadMemorySection() = 0;
    virtual bool loadGlobalSection() = 0;
    virtual bool loadExportSection() = 0;
    virtual bool loadElementSection() = 0;
    virtual bool loadCodeSection() = 0;
    virtual bool loadDataSection() = 0;

    virtual TypeSection* getTypeSection() const = 0;
    virtual ImportSection* getImportSection() const = 0;
    virtual FunctionSection* getFunctionSection() const = 0;
    virtual TableSection* getTableSection() const = 0;
    virtual MemorySection* getMemorySection() const = 0;
    virtual GlobalSection* getGlobalSection() const = 0;
    virtual ExportSection* getExportSection() const = 0;
    virtual ElementSection* getElementSection() const = 0;
    virtual CodeSection* getCodeSection() const = 0;
    virtual DataSection* getDataSection() const = 0;
};
#endif // SECTION_H
