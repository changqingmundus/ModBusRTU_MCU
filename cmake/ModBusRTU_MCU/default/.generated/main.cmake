# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "/home/coder/project/Tools" CACHE PATH "Path to the root of a pack repository.")

function(ModBusRTU_MCU_default_default_XC16_assemble_rule target)
    set(options
        "-g"
        "-mcpu=33CK256MP502"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--defsym=__MPLAB_DEBUGGER_SNAP=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/dsPIC33CK_DFP_1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(ModBusRTU_MCU_default_default_XC16_assemblePreproc_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "-mcpu=33CK256MP502"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--defsym=__MPLAB_DEBUGGER_SNAP=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/dsPIC33CK_DFP_1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(ModBusRTU_MCU_default_default_XC16_compile_rule target)
    set(options
        "-g"
        "-mcpu=33CK256MP502"
        "-mlarge-code"
        "-O0"
        "-msmart-io=1"
        "-Wall"
        "-msfr-warn=off"
        "-mdfp=${PACK_REPO_PATH}/dsPIC33CK_DFP_1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(ModBusRTU_MCU_default_dependentObject_rule target)
    set(options
        "-c"
        "-mcpu=33CK256MP502"
        "-mdfp=${PACK_REPO_PATH}/dsPIC33CK_DFP_1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(ModBusRTU_MCU_default_link_rule target)
    set(options
        "-g"
        "-mcpu=33CK256MP502"
        "-mreserve=data@0x1000:0x101b"
        "-mreserve=data@0x101c:0x101d"
        "-mreserve=data@0x101e:0x101f"
        "-mreserve=data@0x1020:0x1021"
        "-mreserve=data@0x1022:0x1023"
        "-mreserve=data@0x1024:0x1027"
        "-mreserve=data@0x1028:0x104f"
        "-Wl,--script=p33CK256MP502.gld,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_SNAP=1,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/dsPIC33CK_DFP_1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=__DEBUG"
        PRIVATE "__MPLAB_DEBUGGER_SNAP=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(ModBusRTU_MCU_default_bin2hex_rule target)
    add_custom_target(
        ModBusRTU_MCU_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${ModBusRTU_MCU_default_image_name} -a -mdfp=${PACK_REPO_PATH}/dsPIC33CK_DFP_1.15.423/xc16
        WORKING_DIRECTORY ${ModBusRTU_MCU_default_output_dir}
        BYPRODUCTS "${ModBusRTU_MCU_default_output_dir}/${ModBusRTU_MCU_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(ModBusRTU_MCU_default_Bin2Hex ${target})
endfunction()
