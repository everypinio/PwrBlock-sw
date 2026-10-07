add_subdirectory(hal)

# FreeRTOS
add_library(freertos_config INTERFACE)
target_include_directories(
    freertos_config SYSTEM INTERFACE ${CMAKE_SOURCE_DIR}/src/config/freertos
)
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)
target_link_libraries(freertos_config INTERFACE stm32g0b1)
# Select the cross-compile compile PORT
set(FREERTOS_PORT
    "GCC_ARM_CM0"
    CACHE STRING "" FORCE
)
set(FREERTOS_HEAP 4)
add_subdirectory(cmsis_freertos/Source)

# CMSIS V2 FreeRTOS
add_library(cmsis_freertos INTERFACE)
target_include_directories(
    cmsis_freertos SYSTEM INTERFACE cmsis_freertos/CMSIS/RTOS2/FreeRTOS/Include
)
target_sources(
    cmsis_freertos
    INTERFACE cmsis_freertos/CMSIS/RTOS2/FreeRTOS/Source/cmsis_os2.c
)
target_link_libraries(
    cmsis_freertos INTERFACE stm32g0b1 freertos_config freertos_kernel
)

add_library(tinyusb STATIC)

target_sources(
    tinyusb
    PRIVATE tinyusb/src/tusb.c
            tinyusb/src/common/tusb_fifo.c
            tinyusb/src/device/usbd.c
            tinyusb/src/device/usbd_control.c
            tinyusb/src/class/usbtmc/usbtmc_device.c
            tinyusb/src/class/cdc/cdc_device.c
            tinyusb/src/class/vendor/vendor_device.c
)
target_include_directories(tinyusb SYSTEM PUBLIC tinyusb/src)

target_sources(
    tinyusb PUBLIC tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c
)
target_include_directories(tinyusb PUBLIC ${CMAKE_SOURCE_DIR}/src/config)
target_link_libraries(tinyusb PUBLIC stm32g0b1 freertos_config freertos_kernel)

add_subdirectory(stm32_usbpd)

target_link_libraries(stm32_usbpd INTERFACE hal_stm32g0b1)
if(TRACE_USBPD)
    target_compile_definitions(stm32_usbpd INTERFACE _TRACE)
    target_include_directories(
        stm32_usbpd INTERFACE ${CMAKE_SOURCE_DIR}/lib/stm32_usbpd/tracer
    )
    target_sources(
        stm32_usbpd
        INTERFACE ${CMAKE_SOURCE_DIR}/lib/stm32_usbpd/tracer/tracer_emb_hw.c
                  ${CMAKE_SOURCE_DIR}/lib/stm32_usbpd/tracer/tracer_emb.c
    )
endif(TRACE_USBPD)

add_library(scpi_parser STATIC)

target_sources(
    scpi_parser
    PRIVATE scpi_parser/libscpi/src/error.c
            scpi_parser/libscpi/src/expression.c
            scpi_parser/libscpi/src/fifo.c
            scpi_parser/libscpi/src/ieee488.c
            scpi_parser/libscpi/src/lexer.c
            scpi_parser/libscpi/src/minimal.c
            scpi_parser/libscpi/src/parser.c
            scpi_parser/libscpi/src/units.c
            scpi_parser/libscpi/src/utils.c
)
target_include_directories(scpi_parser SYSTEM PUBLIC scpi_parser/libscpi/inc)
target_compile_definitions(scpi_parser PUBLIC USE_FULL_ERROR_LIST=1)
