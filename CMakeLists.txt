cmake_minimum_required(VERSION 3.3)
project(CCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc")

set(COMPILER_BIN_OUTPUT "${CMAKE_SOURCE_DIR}/bin")

set_target_properties(mycc
        PROPERTIES
        DEBUG_POSTFIX -d
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${COMPILER_BIN_OUTPUT}"
        RUNTIME_OUTPUT_DIRECTORY "${COMPILER_BIN_OUTPUT}"
)
