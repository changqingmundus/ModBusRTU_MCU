# 使用 file(GLOB ...) 收集源文件（注意：GLOB 不推荐用于生产，但调试可用）
file(GLOB FreeModbus_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_app/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_core/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_port/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/User/src/*.c
)

# 创建静态库
add_library(FreeModbus STATIC ${FreeModbus_SOURCES})

target_include_directories(FreeModbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_app
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_core
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeModbus_port
)