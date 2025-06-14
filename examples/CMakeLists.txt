add_executable(assetto_corsa assetto_corsa.c)

## Link shared
if (TARGET libracing_shared)
    target_link_libraries(assetto_corsa PRIVATE libracing_shared)
endif()

## Link static
if (TARGET libracing_static)
    target_link_libraries(assetto_corsa PRIVATE libracing_static)
endif()

target_include_directories(assetto_corsa PRIVATE ${CMAKE_SOURCE_DIR}/include)
