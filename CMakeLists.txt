cmake_minimum_required(VERSION 3.21.1)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PWRBLOCK_VERSION)
string(STRIP "${PWRBLOCK_VERSION}" PWRBLOCK_VERSION)

project(
    power_block
    C
    CXX
    ASM
)

include(cmake/gcc-arm-none-eabi.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(USB_MODE
    "tmc"
    CACHE STRING "Select USB device class (cdc or tmc)"
)

# External libraries
add_subdirectory(lib)

# Main app
add_executable(power_block)
include_directories(power_block src)

set(FIRMWARE_VERSION
    "${PWRBLOCK_VERSION}"
    CACHE STRING "Firmware version string"
)
message(STATUS "Firmware version: ${FIRMWARE_VERSION}")
target_compile_definitions(power_block PRIVATE FIRMWARE_VERSION=\"${FIRMWARE_VERSION}\")

add_subdirectory(src)

# Add extra config for hex/bin/list files generation
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("Debug: will create asm listing.")

    set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)

    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -h -S $<TARGET_FILE:${CMAKE_PROJECT_NAME}> >
                "${CMAKE_PROJECT_NAME}.list"
        VERBATIM
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Generating .bin and .hex files for ${PROJECT_NAME}"
)
