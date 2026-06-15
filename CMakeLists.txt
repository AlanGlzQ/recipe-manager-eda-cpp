cmake_minimum_required(VERSION 3.14)
project(RecetarioEDA CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Forzar MD/MDd (DLL runtime) en MSVC para todo el proyecto ───────────────
# Esto evita el conflicto MTd vs MDd con GTest en Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# ─── Ejecutable principal ────────────────────────────────────────────────────
file(GLOB SRC_FILES src/*.cpp)
add_executable(recetario ${SRC_FILES})
target_include_directories(recetario PRIVATE include)

if(WIN32)
    target_compile_options(recetario PRIVATE /utf-8)
endif()

# ─── Google Test ─────────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
# Forzar mismo runtime en GTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Fuentes compartidas (sin main.cpp)
file(GLOB LIB_SOURCES src/*.cpp)
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Archivos de test
file(GLOB TEST_FILES tests/*.cpp)

add_executable(recetario_tests ${TEST_FILES} ${LIB_SOURCES})
target_include_directories(recetario_tests PRIVATE include)
target_link_libraries(recetario_tests gtest_main)

if(WIN32)
    target_compile_options(recetario_tests PRIVATE /utf-8)
endif()

include(GoogleTest)
gtest_discover_tests(recetario_tests)