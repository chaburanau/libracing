cmake_minimum_required(VERSION 3.31)
project(libracing C)

set(CMAKE_C_STANDARD 11)

## Versioning
set(LIBRACING_VERSION_MAJOR 1)
set(LIBRACING_VERSION_MINOR 0)
set(LIBRACING_VERSION_PATCH 0)
set(LIBRACING_VERSION "${LIBRACING_VERSION_MAJOR}.${LIBRACING_VERSION_MINOR}.${LIBRACING_VERSION_PATCH}")

## Sources
file(GLOB LIBRACING_SRC src/*.c)
file(GLOB LIBRACING_HEADERS include/libracing/*.h)

## Options
option(LIBRACING_BUILD_SHARED "Build shared library" ON)
option(LIBRACING_BUILD_STATIC "Build static library" ON)

## Shared Library
if (LIBRACING_BUILD_SHARED)
    add_library(libracing_shared SHARED ${LIBRACING_SRC})
    target_include_directories(libracing_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(libracing_shared PROPERTIES OUTPUT_NAME "libracing")
    target_compile_definitions(libracing_shared PRIVATE LIBRACING_EXPORTS)
endif()

## Static Library
if (LIBRACING_BUILD_STATIC)
    add_library(libracing_static STATIC ${LIBRACING_SRC})
    target_include_directories(libracing_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(libracing_static PROPERTIES OUTPUT_NAME "libracing")
endif()

## Installation
install(DIRECTORY include/libracing DESTINATION include)

if (LIBRACING_BUILD_SHARED)
    install(TARGETS libracing_shared
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib)
endif()

if (LIBRACING_BUILD_STATIC)
    install(TARGETS libracing_static
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin)
endif()

set_target_properties(libracing_shared PROPERTIES VERSION ${LIBRACING_VERSION} SOVERSION ${LIBRACING_VERSION_MAJOR})
