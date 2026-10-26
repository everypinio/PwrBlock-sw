add_library(stm32g0b1 INTERFACE)
target_compile_definitions(stm32g0b1 INTERFACE STM32G0B1xx)
target_compile_definitions(stm32g0b1 INTERFACE CFG_TUSB_MCU=OPT_MCU_STM32G0)
target_include_directories(
    stm32g0b1 SYSTEM
    INTERFACE cmsis_6/CMSIS/Core/Include cmsis_device_g0/Include
              cmsis_6/CMSIS/RTOS2/Include
)

add_library(hal_stm32g0b1 INTERFACE)
target_compile_definitions(
    hal_stm32g0b1 INTERFACE USE_FULL_LL_DRIVER USE_HAL_DRIVER
)
target_sources(
    hal_stm32g0b1
    INTERFACE stm32g0xx_hal_driver/Src/stm32g0xx_hal.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_adc.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_adc_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_cortex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_pwr.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_pwr_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_rcc.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_rcc_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_gpio.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_uart.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_uart_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_dma.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_tim.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_tim_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_spi.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_i2c.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_i2c_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_iwdg.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_pcd.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_pcd_ex.c
              stm32g0xx_hal_driver/Src/stm32g0xx_hal_dac.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_adc.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_rcc.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_dma.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_utils.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_exti.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_crs.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_ucpd.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_usb.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_gpio.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_tim.c
              stm32g0xx_hal_driver/Src/stm32g0xx_ll_usart.c
)
target_include_directories(
    hal_stm32g0b1 SYSTEM INTERFACE stm32g0xx_hal_driver/Inc
)
target_include_directories(
    hal_stm32g0b1 INTERFACE ${CMAKE_SOURCE_DIR}/src/bsp/periph_setup/Inc
)
target_link_libraries(hal_stm32g0b1 INTERFACE stm32g0b1)
