# The following variables contains the files used by the different stages of the build process.
set(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemble
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/flash/src/flash_asm.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/dmt_asm.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/where_was_i.s")
set_source_files_properties(${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mb.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbascii.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbcrc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbfunccoils.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbfuncdiag.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbfuncdisc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbfuncholding.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbfuncinput.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbfuncother.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbrtu.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbtcp.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_core/src/mbutils.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_port/src/portevent.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_port/src/portserial.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/FreeModbus_port/src/porttimer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../FreeModbus/User/src/mb_user.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/driver/src/cleardata.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/driver/src/encoder.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/MB_User_Config/src/MB_User_Config.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/dee/src/dee.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/flash/src/flash.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/config_bits.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/dmt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/interrupt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/pins.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/reset.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/system.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/system/src/traps.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/timer/src/delay.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/timer/src/sccp1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/timer/src/sccp2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ModBusRTU.mcc/mcc_generated_files/uart/src/uart1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c")
set_source_files_properties(${ModBusRTU_MCU_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_link)
set(ModBusRTU_MCU_default_default_XC16_FILE_TYPE_bin2hex)
set(ModBusRTU_MCU_default_image_name "default.elf")
set(ModBusRTU_MCU_default_image_base_name "default")

# The output directory of the final image.
set(ModBusRTU_MCU_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/ModBusRTU_MCU")

# The full path to the final image.
set(ModBusRTU_MCU_default_full_path_to_image ${ModBusRTU_MCU_default_output_dir}/${ModBusRTU_MCU_default_image_name})
