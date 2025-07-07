cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project("ds-rtti" LANGUAGES C CXX)

list(APPEND CMAKE_PREFIX_PATH "./_thirdparty/${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable(ds-rtti ${sources})

# Options
set(ds_clang_options -Wall -Wextra -Wpedantic -Werror
    -Wno-unused-function
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-private-field
    -fno-rtti
)
set(ds_compile_definitions
    _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    _CRT_SECURE_NO_WARNINGS
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    list(APPEND ds_clang_options /EHs)
    list(APPEND ds_compile_definitions _WIN32_WINNT=0x0601)
endif ()

# Files
file(GLOB_RECURSE sources
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
)

# Configure
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${ds_compile_definitions})
target_compile_options(${PROJECT_NAME} PRIVATE ${ds_clang_options})

# Third party
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(LLVM CONFIG REQUIRED)
find_package(Clang REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs option support core)
target_link_libraries(${PROJECT_NAME} PRIVATE
    clangTooling
    clangFrontend
    clangASTMatchers
    clangAST
    clangBasic
    clangLex
    ${llvm_libs}
)
