#ifndef SECTION_H 
#define SECTION_H

#include "type_section.h"
#include "import_section.h"
#include "function_section.h"
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
    virtual bool loadGlobalSection() = 0;
    virtual bool loadExportSection() = 0;
    virtual bool loadElementSection() = 0;
    virtual bool loadCodeSection() = 0;
    virtual bool loadDataSection() = 0;

    virtual TypeSection* getTypeSection() = 0;
    virtual ImportSection* getImportSection() = 0;
    virtual FunctionSection* getFunctionSection() = 0;
    virtual GlobalSection* getGlobalSection() = 0;
    virtual ExportSection* getExportSection() = 0;
    virtual ElementSection* getElementSection() = 0;
    virtual CodeSection* getCodeSection() = 0;
    virtual DataSection* getDataSection() = 0;
};
#endif // SECTION_H
