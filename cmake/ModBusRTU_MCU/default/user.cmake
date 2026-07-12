include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/dee
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/uart
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/timer
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/driver/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_port
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/User
)

add_subdirectory(
"${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus"
"${CMAKE_BINARY_DIR}/FreeModbus"
)

list(REMOVE_ITEM PROJECT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_app/src/demo.c"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成 compile_commands.json，方便 VSCode 等工具使用
