CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT ("WASM-INTERPRETER")

SET (TARGET "wasmIntrp")
SET (SRC_SECTION
    section.cpp
    section_v1.cpp
    instruction.cpp
    init.cpp
    type_section.cpp
    import_section.cpp
    function_section.cpp
    global_section.cpp
    export_section.cpp
    element_section.cpp
    data_section.cpp
)

SET (SRC_FILES
    main.cpp
    memory.cpp
    loader.cpp
    type.cpp
    module.cpp
    ${SRC_SECTION}
)

ADD_EXECUTABLE (${TARGET} ${SRC_FILES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES
    COMPILE_FLAGS "-std=c++11 -g"
)
