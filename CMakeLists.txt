CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT ("WAIN")

SET (TARGET "wain")
SET (SRC_SECTION
    src/section/section.cpp
    src/section/section_v1.cpp
    src/section/instruction.cpp
    src/section/init_expr.cpp
    src/section/type_section.cpp
    src/section/import_section.cpp
    src/section/function_section.cpp
    src/section/table_section.cpp
    src/section/memory_section.cpp
    src/section/global_section.cpp
    src/section/export_section.cpp
    src/section/element_section.cpp
    src/section/code_section.cpp
    src/section/data_section.cpp
)

SET (SRC_INTERPRETER
    src/interpreter/interpreter.cpp
    src/interpreter/stack.cpp
)

SET (SRC_FILES
    src/main.cpp
    src/loader/loader.cpp
    src/memory/memory.cpp
    src/type.cpp
    src/module.cpp
    ${SRC_INTERPRETER}
    ${SRC_SECTION}
)

ADD_EXECUTABLE (${TARGET} ${SRC_FILES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES
    COMPILE_FLAGS "-std=c++11 -g"
)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE src/ src/interpreter src/loader src/memory src/section)
