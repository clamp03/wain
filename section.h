#ifndef SECTION_H 
#define SECTION_H

#include "memory.h"
#include "loader.h"
#include "type_section.h"
#include "import_section.h"
#include "function_section.h"

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
    virtual bool loadGlobalSection() = 0;
    virtual bool loadExportSection() = 0;
    virtual bool loadElementSection() = 0;
    virtual bool loadCodeSection() = 0;
    virtual bool loadDataSection() = 0;
};

class TypeSection;

class SectionsV1 : public Sections {
public:
    explicit SectionsV1(Loader& l);
    virtual ~SectionsV1();
    virtual bool load();
    virtual bool loadTypeSection();
    virtual bool loadImportSection();
    virtual bool loadFunctionSection();
    virtual bool loadGlobalSection();
    virtual bool loadExportSection();
    virtual bool loadElementSection();
    virtual bool loadCodeSection();
    virtual bool loadDataSection();

private:
    MemoryManager mem_;
    Loader& loader_;
    TypeSection* type_section_;
    ImportSection* import_section_;
    FunctionSection* function_section_;
};

#endif // SECTION_H
