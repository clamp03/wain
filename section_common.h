#ifndef SECTION_COMMON_H
#define SECTION_COMMON_H

#include "common.h"
#include "loader.h"
#include "memory.h"

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

#endif // SECTION_COMMON_H
