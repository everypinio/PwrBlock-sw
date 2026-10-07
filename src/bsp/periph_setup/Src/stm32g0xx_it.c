/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g0xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_it.h"
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <device/usbd.h>
#include <usbpd_hw_if.h>

#include "bsp/drivers/ext_conn/i2c_bus_it.h"

// UCPD Trace module
#ifdef _TRACE
#include <tracer_emb.h>
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef  hadc1;
extern I2C_HandleTypeDef  hi2c1;
extern I2C_HandleTypeDef  hi2c2;
extern SPI_HandleTypeDef  hspi1;
extern DMA_HandleTypeDef  hdma_tim4_ch2;
extern UART_HandleTypeDef huart3;
extern PCD_HandleTypeDef  hpcd_USB_DRD_FS;
extern TIM_HandleTypeDef  htim2;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {}
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        __asm("BKPT #0\n");
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles EXTI line 0 and line 1 interrupts.
 */
void EXTI0_1_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI0_1_IRQn 0 */

    /* USER CODE END EXTI0_1_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(BUTTON_Pin);
    /* USER CODE BEGIN EXTI0_1_IRQn 1 */

    /* USER CODE END EXTI0_1_IRQn 1 */
}

/**
 * @brief This function handles EXTI line 4 to 15 interrupts.
 */
void EXTI4_15_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_15_IRQn 0 */

    /* USER CODE END EXTI4_15_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(INA_ALERT_Pin);
    /* USER CODE BEGIN EXTI4_15_IRQn 1 */

    /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
 * @brief This function handles USB, UCPD1 and UCPD2 global interrupts.
 */
void USB_UCPD1_2_IRQHandler(void)
{
    /* USER CODE BEGIN USB_UCPD1_2_IRQn 0 */

    /* USER CODE END USB_UCPD1_2_IRQn 0 */
    /* USER CODE BEGIN USB_UCPD1_2_IRQn 1 */

    // USB PD by stm-ucpd
    USBPD_PORT0_IRQHandler();

    // USB FS by tinyUSB
    tud_int_handler(0);
    /* USER CODE END USB_UCPD1_2_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel 1 interrupt.
 */
void DMA1_Channel1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

    /* USER CODE END DMA1_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_tim4_ch2);
    /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

    /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
 */
void DMA1_Channel2_3_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

    /* USER CODE END DMA1_Channel2_3_IRQn 0 */
    /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */
#ifdef _TRACE
    TRACER_EMB_IRQHandlerDMA();
#endif
    /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
 * @brief This function handles DMA1 Ch4 to Ch7, DMA2 Ch1 to Ch5 and DMAMUX1
 * Overrun Interrupts.
 */
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn 0 */

    /* USER CODE END DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn 0 */
    /* USER CODE BEGIN DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn 1 */

    /* USER CODE END DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn 1 */
}

/**
 * @brief This function handles ADC1, COMP1,COMP2, COMP3 Interrupts (combined
 * with EXTI 17 & 18).
 */
void ADC1_COMP_IRQHandler(void)
{
    /* USER CODE BEGIN ADC1_COMP_IRQn 0 */

    /* USER CODE END ADC1_COMP_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc1);
    /* USER CODE BEGIN ADC1_COMP_IRQn 1 */

    /* USER CODE END ADC1_COMP_IRQn 1 */
}

/**
 * @brief This function handles TIM2 global interrupt.
 */
void TIM2_IRQHandler(void)
{
    /* USER CODE BEGIN TIM2_IRQn 0 */

    /* USER CODE END TIM2_IRQn 0 */
    HAL_TIM_IRQHandler(&htim2);
    /* USER CODE BEGIN TIM2_IRQn 1 */

    /* USER CODE END TIM2_IRQn 1 */
}

/**
 * @brief This function handles I2C1 event global interrupt / I2C1 wake-up
 * interrupt through EXTI line 23.
 */
void I2C1_IRQHandler(void)
{
    /* USER CODE BEGIN I2C1_IRQn 0 */

    /* USER CODE END I2C1_IRQn 0 */
    if (hi2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
        HAL_I2C_ER_IRQHandler(&hi2c1);
    }
    else {
        HAL_I2C_EV_IRQHandler(&hi2c1);
    }
    /* USER CODE BEGIN I2C1_IRQn 1 */

    /* USER CODE END I2C1_IRQn 1 */
}

/**
 * @brief This function handles I2C2, I2C3 Interrupt (combined with EXTI 24 and
 * EXTI 22).
 */
void I2C2_3_IRQHandler(void)
{
    /* USER CODE BEGIN I2C2_3_IRQn 0 */

    /* USER CODE END I2C2_3_IRQn 0 */
    if (hi2c2.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
        HAL_I2C_ER_IRQHandler(&hi2c2);
    }
    else {
        HAL_I2C_EV_IRQHandler(&hi2c2);
    }
    /* USER CODE BEGIN I2C2_3_IRQn 1 */
    i2c_bus_irq_handler();
    /* USER CODE END I2C2_3_IRQn 1 */
}

/**
 * @brief This function handles SPI1/I2S1 Interrupt.
 */
void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN SPI1_IRQn 0 */

    /* USER CODE END SPI1_IRQn 0 */
    HAL_SPI_IRQHandler(&hspi1);
    /* USER CODE BEGIN SPI1_IRQn 1 */

    /* USER CODE END SPI1_IRQn 1 */
}

/**
 * @brief This function handles USART3, USART4, USART5, USART6, LPUART1 globlal
 * Interrupts (combined with EXTI 28).
 */
void USART3_4_5_6_LPUART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART3_4_5_6_LPUART1_IRQn 0 */

    /* USER CODE END USART3_4_5_6_LPUART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart3);
    /* USER CODE BEGIN USART3_4_5_6_LPUART1_IRQn 1 */
#ifdef _TRACE
    TRACER_EMB_IRQHandlerUSART();
#endif
    /* USER CODE END USART3_4_5_6_LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
