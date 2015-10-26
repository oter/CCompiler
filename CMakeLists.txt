cmake_minimum_required(VERSION 3.3)
project(CCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc")

set(COMPILER_BIN_OUTPUT "${CMAKE_SOURCE_DIR}/bin")

set(BOOST_ROOT "$ENV{MINGW_HOME}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED COMPONENTS regex wave filesystem system)

############# COMPILER #############
set(COMPILER_SOURCE_FILES
        main.cxx
        preprocessor/PreprocessFile.cxx
        preprocessor/PreprocessFile.hpp
        utils/CommentsCleaner.cxx
        utils/CommentsCleaner.hpp
        preprocessor/PreprocessedSlice.cxx
        preprocessor/PreprocessedSlice.hpp
        preprocessor/PreprocessorDefine.cxx
        preprocessor/PreprocessorDefine.hpp
        preprocessor/Preprocessor.cxx
        preprocessor/Preprocessor.hpp
        utils/FilesResolver.cxx
        utils/FilesResolver.hpp
        utils/FSettings.cxx
        utils/FSettings.hpp
        utils/FSingleton.hpp
)
add_executable(mycc ${COMPILER_SOURCE_FILES})

set_target_properties(mycc
        PROPERTIES
        DEBUG_POSTFIX -d
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${COMPILER_BIN_OUTPUT}"
        RUNTIME_OUTPUT_DIRECTORY "${COMPILER_BIN_OUTPUT}"
)
target_include_directories(mycc PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(mycc ${Boost_LIBRARIES})




