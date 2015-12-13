cmake_minimum_required(VERSION 3.3)
project(CCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc")

set(COMPILER_BIN_OUTPUT "${CMAKE_SOURCE_DIR}/bin")

set(BOOST_ROOT "$ENV{MINGW_HOME}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
#find_package(Boost REQUIRED COMPONENTS regex wave filesystem system)

############# COMPILER #############
set(COMPILER_SOURCE_FILES
        grammar/parser.cxx
        grammar/parser.hxx
        grammar/lexer.cxx
        main.cxx
        # utils/CommentsCleaner.cxx
        # utils/CommentsCleaner.hpp
        # utils/FilesResolver.cxx
        # utils/FilesResolver.hpp
        # utils/FSettings.cxx
        # utils/FSettings.hpp
        # utils/FSingleton.hpp
        asm/asm_context.hpp
        asm/asm_context.cxx
        asm/asm_variable.cxx
        asm/asm_variable.hpp
        asm/asm_scope.hpp
        asm/asm_scope.cxx
        asm/asm_generator.hpp
        asm/asm_generator.cxx
        tree_syntax/tree_syntax.hpp
        tree_syntax/tree_syntax.cxx
        tree_syntax/tree_syntax_helper.hpp
        tree_syntax/tree_syntax_helper.cxx)
add_executable(myc ${COMPILER_SOURCE_FILES})

set_target_properties(myc
        PROPERTIES
        DEBUG_POSTFIX -d
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${COMPILER_BIN_OUTPUT}"
        RUNTIME_OUTPUT_DIRECTORY "${COMPILER_BIN_OUTPUT}"
)
target_include_directories(myc PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(myc ${Boost_LIBRARIES})




