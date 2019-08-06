#ifndef SECTION_V1_H
#define SECTION_V1_h

#include "memory.h"
#include "loader.h"
#include "type_section.h"
#include "import_section.h"
#include "function_section.h"
#include "table_section.h"
#include "global_section.h"
#include "export_section.h"
#include "element_section.h"
#include "code_section.h"
#include "data_section.h"
#include "section.h"

class SectionsV1 : public Sections {
public:
    explicit SectionsV1(Loader& l);
    virtual ~SectionsV1();
    virtual bool load();
    virtual bool loadTypeSection();
    virtual bool loadImportSection();
    virtual bool loadFunctionSection();
    virtual bool loadTableSection();
    virtual bool loadMemorySection();
    virtual bool loadGlobalSection();
    virtual bool loadExportSection();
    virtual bool loadElementSection();
    virtual bool loadCodeSection();
    virtual bool loadDataSection();

    virtual TypeSection* getTypeSection();
    virtual ImportSection* getImportSection();
    virtual FunctionSection* getFunctionSection();
    virtual TableSection* getTableSection();
    virtual MemorySection* getMemorySection();
    virtual GlobalSection* getGlobalSection();
    virtual ExportSection* getExportSection();
    virtual ElementSection* getElementSection();
    virtual CodeSection* getCodeSection();
    virtual DataSection* getDataSection();

private:
    InitExpr* loadInitExpr();

    MemoryManager mem_;
    Loader& loader_;
    TypeSection* type_section_;
    ImportSection* import_section_;
    FunctionSection* function_section_;
    TableSection* table_section_;
    MemorySection* memory_section_;
    GlobalSection* global_section_;
    ExportSection* export_section_;
    ElementSection* element_section_;
    CodeSection* code_section_;
    DataSection* data_section_;
};
#endif // SECTION_V1_H
