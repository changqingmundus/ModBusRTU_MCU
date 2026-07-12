file(GLOB FreeModbus_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_core/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_port/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User/src/*.c
)

add_library(FreeModbus STATIC ${FreeModbus_SOURCES})

target_compile_options(FreeModbus PRIVATE
   "-mcpu=33CK256MP502"
)

target_include_directories(FreeModbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_core
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_port
)