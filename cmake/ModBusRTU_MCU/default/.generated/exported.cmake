set(DEPENDENT_MP_BIN2HEXModBusRTU_MCU_default_zcwHaz8G "d:/Microchip/xc16/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_zcwHaz8G "${CMAKE_CURRENT_LIST_DIR}/../../../../out/ModBusRTU_MCU/default.elf")
set(DEPENDENT_TARGET_DIRModBusRTU_MCU_default_zcwHaz8G "${CMAKE_CURRENT_LIST_DIR}/../../../../out/ModBusRTU_MCU")
set(DEPENDENT_BYPRODUCTSModBusRTU_MCU_default_zcwHaz8G ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_zcwHaz8G}/${sourceFileNameModBusRTU_MCU_default_zcwHaz8G}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_zcwHaz8G}/${sourceFileNameModBusRTU_MCU_default_zcwHaz8G}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXModBusRTU_MCU_default_zcwHaz8G} ${DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_zcwHaz8G} --image ${sourceFileNameModBusRTU_MCU_default_zcwHaz8G} ${addressModBusRTU_MCU_default_zcwHaz8G} ${modeModBusRTU_MCU_default_zcwHaz8G} -mdfp=C:/Users/ChangqingMundus/.mchp_packs/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_zcwHaz8G}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFModBusRTU_MCU_default_zcwHaz8G})
add_custom_target(
    dependent_produced_source_artifactModBusRTU_MCU_default_zcwHaz8G 
    DEPENDS ${DEPENDENT_TARGET_DIRModBusRTU_MCU_default_zcwHaz8G}/${sourceFileNameModBusRTU_MCU_default_zcwHaz8G}.s
    )
