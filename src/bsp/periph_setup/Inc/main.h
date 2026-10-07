/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_ucpd.h"
#include "stm32g0xx_ll_utils.h"

#include "stm32g0xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IO7_Pin                GPIO_PIN_13
#define IO7_GPIO_Port          GPIOC
#define VUSB_PWR_ADC_Pin       GPIO_PIN_0
#define VUSB_PWR_ADC_GPIO_Port GPIOA
#define VUSB_PC_ADC_Pin        GPIO_PIN_1
#define VUSB_PC_ADC_GPIO_Port  GPIOA
#define NTC_ADC_Pin            GPIO_PIN_2
#define NTC_ADC_GPIO_Port      GPIOA
#define MCU_ISET_DAC_Pin       GPIO_PIN_4
#define MCU_ISET_DAC_GPIO_Port GPIOA
#define MCU_ATRK_DAC_Pin       GPIO_PIN_5
#define MCU_ATRK_DAC_GPIO_Port GPIOA
#define INA_SPI_MISO_Pin       GPIO_PIN_6
#define INA_SPI_MISO_GPIO_Port GPIOA
#define I2C2_SCL_Pin           GPIO_PIN_7
#define I2C2_SCL_GPIO_Port     GPIOA
#define USART3_RX_Pin          GPIO_PIN_0
#define USART3_RX_GPIO_Port    GPIOB
#define BUTTON_Pin             GPIO_PIN_1
#define BUTTON_GPIO_Port       GPIOB
#define BUTTON_EXTI_IRQn       EXTI0_1_IRQn
#define USART3_TX_Pin          GPIO_PIN_2
#define USART3_TX_GPIO_Port    GPIOB
#define TPS_PG_Pin             GPIO_PIN_11
#define TPS_PG_GPIO_Port       GPIOB
#define INA_ALERT_Pin          GPIO_PIN_12
#define INA_ALERT_GPIO_Port    GPIOB
#define INA_ALERT_EXTI_IRQn    EXTI4_15_IRQn
#define LM_EN_Pin              GPIO_PIN_13
#define LM_EN_GPIO_Port        GPIOB
#define UCPD1_CC2_Pin          GPIO_PIN_15
#define UCPD1_CC2_GPIO_Port    GPIOB
#define UCPD1_CC1_Pin          GPIO_PIN_8
#define UCPD1_CC1_GPIO_Port    GPIOA
#define I2C1_SCL_Pin           GPIO_PIN_9
#define I2C1_SCL_GPIO_Port     GPIOA
#define IO6_Pin                GPIO_PIN_6
#define IO6_GPIO_Port          GPIOC
#define IO5_Pin                GPIO_PIN_7
#define IO5_GPIO_Port          GPIOC
#define I2C1_SDA_Pin           GPIO_PIN_10
#define I2C1_SDA_GPIO_Port     GPIOA
#define USBFS_DM_Pin           GPIO_PIN_11
#define USBFS_DM_GPIO_Port     GPIOA
#define USBFS_DP_Pin           GPIO_PIN_12
#define USBFS_DP_GPIO_Port     GPIOA
#define INA_SPI_NSS_Pin        GPIO_PIN_15
#define INA_SPI_NSS_GPIO_Port  GPIOA
#define IO1_Pin                GPIO_PIN_0
#define IO1_GPIO_Port          GPIOD
#define IO2_Pin                GPIO_PIN_1
#define IO2_GPIO_Port          GPIOD
#define IO3_Pin                GPIO_PIN_2
#define IO3_GPIO_Port          GPIOD
#define IO4_Pin                GPIO_PIN_3
#define IO4_GPIO_Port          GPIOD
#define INA_SPI_SCK_Pin        GPIO_PIN_3
#define INA_SPI_SCK_GPIO_Port  GPIOB
#define I2C2_SDA_Pin           GPIO_PIN_4
#define I2C2_SDA_GPIO_Port     GPIOB
#define INA_SPI_MOSI_Pin       GPIO_PIN_5
#define INA_SPI_MOSI_GPIO_Port GPIOB
#define DTRK_PWM_Pin           GPIO_PIN_6
#define DTRK_PWM_GPIO_Port     GPIOB
#define ADDR_LED_PWM_Pin       GPIO_PIN_7
#define ADDR_LED_PWM_GPIO_Port GPIOB
#define USB_SW1_Pin            GPIO_PIN_8
#define USB_SW1_GPIO_Port      GPIOB
#define USB_SW2_Pin            GPIO_PIN_9
#define USB_SW2_GPIO_Port      GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
