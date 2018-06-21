#ifndef SECTION_H 
#define SECTION_H

#include "common.h"
#include "loader.h"
#include "memory.h"

using namespace std;

class Sections {
public:
    virtual bool load(Loader& l) = 0;
    virtual bool loadTypeSection(Loader& l) = 0;
    virtual bool loadImportSection(Loader& l) = 0;
    virtual bool loadFunctionSection(Loader& l) = 0;
    virtual bool loadGlobalSection(Loader& l) = 0;
    virtual bool loadExportSection(Loader& l) = 0;
    virtual bool loadElementSection(Loader& l) = 0;
    virtual bool loadCodeSection(Loader& l) = 0;
    virtual bool loadDataSection(Loader& l) = 0;

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

    enum class ExternalKind {
        Function = 0,
        Table,
        Memory,
        Global
    };
};

class SectionsV1: public Sections {
public:
    virtual bool load(Loader& l);
    virtual bool loadTypeSection(Loader& l);
    virtual bool loadImportSection(Loader& l);
    virtual bool loadFunctionSection(Loader& l);
    virtual bool loadGlobalSection(Loader& l);
    virtual bool loadExportSection(Loader& l);
    virtual bool loadElementSection(Loader& l);
    virtual bool loadCodeSection(Loader& l);
    virtual bool loadDataSection(Loader& l);

private:
    MemoryManager mem;
};

#endif // SECTION_H
