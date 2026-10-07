add_library(drivers INTERFACE)
target_sources(
    drivers
    INTERFACE led/led.c
              log_uart/log_uart.c
              uid/uid.c
              lm51772/lm51772.c
              ws2812b/ws2812b.c
              ina229/ina229.c
              dac/dac.c
              24aa02/24aa02.c
              exti/exti.c
              ext_conn/digital_gpio.c
              ext_conn/i2c_bus.c
)
