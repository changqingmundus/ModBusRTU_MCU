set(DEPENDENT_MP_BIN2HEXModBusRTU_MCU_default_7gsMsF5d "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_7gsMsF5d ${CMAKE_CURRENT_LIST_DIR}/../../../../out/ModBusRTU_MCU/default.elf)
set(DEPENDENT_TARGET_DIRModBusRTU_MCU_default_7gsMsF5d ${CMAKE_CURRENT_LIST_DIR}/../../../../out/ModBusRTU_MCU)
set(DEPENDENT_BYPRODUCTSModBusRTU_MCU_default_7gsMsF5d ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_7gsMsF5d}/${sourceFileNameModBusRTU_MCU_default_7gsMsF5d}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_7gsMsF5d}/${sourceFileNameModBusRTU_MCU_default_7gsMsF5d}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXModBusRTU_MCU_default_7gsMsF5d} ${DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_7gsMsF5d} --image ${sourceFileNameModBusRTU_MCU_default_7gsMsF5d} ${addressModBusRTU_MCU_default_7gsMsF5d} ${modeModBusRTU_MCU_default_7gsMsF5d} -mdfp=C:/Users/Administrator/.mchp_packs/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_7gsMsF5d}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_7gsMsF5d})
add_custom_target(
    dependent_produced_source_artifactModBusRTU_MCU_default_7gsMsF5d 
    DEPENDS ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_7gsMsF5d}/${sourceFileNameModBusRTU_MCU_default_7gsMsF5d}.s
    )
