add_library(usbctrl INTERFACE)

if(USB_MODE STREQUAL "tmc")
    add_subdirectory(usbtmc)
    message(STATUS "Configuring build for USB TMC mode...")
    target_compile_definitions(tinyusb PUBLIC USB_MODE_TMC)
    target_compile_definitions(usbctrl INTERFACE USB_MODE_TMC)
    target_link_libraries(usbctrl INTERFACE usbtmc)
elseif(USB_MODE STREQUAL "cdc")
    add_subdirectory(usbcdc)
    message(STATUS "Configuring build for USB CDC (Serial) mode...")
    target_compile_definitions(tinyusb PUBLIC USB_MODE_CDC)
    target_compile_definitions(usbctrl INTERFACE USB_MODE_CDC)
    target_link_libraries(usbctrl INTERFACE usbcdc)
else()
    message(
        FATAL_ERROR
            "Invalid USB_MODE '${USB_MODE}'. Please choose 'cdc' or 'tmc'."
    )
endif()

target_sources(usbctrl INTERFACE tud_app.c tud_task.c usbctl.c)
