include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(ModBusRTU_MCU_default_library_list )

# Handle files with suffix s, for group default-XC16
if(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemble)
add_library(ModBusRTU_MCU_default_default_XC16_assemble OBJECT ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemble})
    ModBusRTU_MCU_default_default_XC16_assemble_rule(ModBusRTU_MCU_default_default_XC16_assemble)
    list(APPEND ModBusRTU_MCU_default_library_list "$<TARGET_OBJECTS:ModBusRTU_MCU_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(ModBusRTU_MCU_default_default_XC16_assemblePreproc OBJECT ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemblePreproc})
    ModBusRTU_MCU_default_default_XC16_assemblePreproc_rule(ModBusRTU_MCU_default_default_XC16_assemblePreproc)
    list(APPEND ModBusRTU_MCU_default_library_list "$<TARGET_OBJECTS:ModBusRTU_MCU_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_compile)
add_library(ModBusRTU_MCU_default_default_XC16_compile OBJECT ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_compile})
    ModBusRTU_MCU_default_default_XC16_compile_rule(ModBusRTU_MCU_default_default_XC16_compile)
    list(APPEND ModBusRTU_MCU_default_library_list "$<TARGET_OBJECTS:ModBusRTU_MCU_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_dependentObject)
add_library(ModBusRTU_MCU_default_default_XC16_dependentObject OBJECT ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_dependentObject})
    ModBusRTU_MCU_default_default_XC16_dependentObject_rule(ModBusRTU_MCU_default_default_XC16_dependentObject)
    list(APPEND ModBusRTU_MCU_default_library_list "$<TARGET_OBJECTS:ModBusRTU_MCU_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_bin2hex)
add_library(ModBusRTU_MCU_default_default_XC16_bin2hex OBJECT ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_bin2hex})
    ModBusRTU_MCU_default_default_XC16_bin2hex_rule(ModBusRTU_MCU_default_default_XC16_bin2hex)
    list(APPEND ModBusRTU_MCU_default_library_list "$<TARGET_OBJECTS:ModBusRTU_MCU_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(ModBusRTU_MCU_default_image_zcwHaz8G ${ModBusRTU_MCU_default_library_list})

set_target_properties(ModBusRTU_MCU_default_image_zcwHaz8G PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${ModBusRTU_MCU_default_output_dir}")
target_link_libraries(ModBusRTU_MCU_default_image_zcwHaz8G PRIVATE ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
ModBusRTU_MCU_default_link_rule( ModBusRTU_MCU_default_image_zcwHaz8G)

# Call bin2hex function from the rule file
ModBusRTU_MCU_default_bin2hex_rule(ModBusRTU_MCU_default_image_zcwHaz8G)

