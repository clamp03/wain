CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT ("WASM-INTERPRETER")

SET (TARGET "wasmIntrp")

SET (SRC_FILES
    main.cpp
    memory.cpp
    loader.cpp
    section.cpp
    module.cpp
)

ADD_EXECUTABLE (${TARGET} ${SRC_FILES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES
    COMPILE_FLAGS "-std=c++11"
)
