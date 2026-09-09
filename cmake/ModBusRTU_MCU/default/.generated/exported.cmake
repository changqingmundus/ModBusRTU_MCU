set(DEPENDENT_MP_BIN2HEXModBusRTU_MCU_default_ic_cEg9l "d:/Microchip/xc16/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_ic_cEg9l ${CMAKE_CURRENT_LIST_DIR}/../../../../out/ModBusRTU_MCU/default.elf)
set(DEPENDENT_TARGET_DIRModBusRTU_MCU_default_ic_cEg9l ${CMAKE_CURRENT_LIST_DIR}/../../../../out/ModBusRTU_MCU)
set(DEPENDENT_BYPRODUCTSModBusRTU_MCU_default_ic_cEg9l ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_ic_cEg9l}/${sourceFileNameModBusRTU_MCU_default_ic_cEg9l}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_ic_cEg9l}/${sourceFileNameModBusRTU_MCU_default_ic_cEg9l}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXModBusRTU_MCU_default_ic_cEg9l} ${DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_ic_cEg9l} --image ${sourceFileNameModBusRTU_MCU_default_ic_cEg9l} ${addressModBusRTU_MCU_default_ic_cEg9l} ${modeModBusRTU_MCU_default_ic_cEg9l} -mdfp=C:/Users/wsh/.mchp_packs/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_ic_cEg9l}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_ic_cEg9l})
add_custom_target(
    dependent_produced_source_artifactModBusRTU_MCU_default_ic_cEg9l 
    DEPENDS ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_ic_cEg9l}/${sourceFileNameModBusRTU_MCU_default_ic_cEg9l}.s
    )
