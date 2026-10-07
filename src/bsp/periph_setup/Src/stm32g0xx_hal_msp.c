/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32g0xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
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
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_tim4_ch2;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{

    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /* System interrupt init*/

    /** Disable the Internal Voltage Reference buffer
     */
    HAL_SYSCFG_DisableVREFBUF();

    /** Configure the internal voltage reference buffer high impedance mode
     */
    HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_ENABLE
    );

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
     */
    HAL_SYSCFG_StrobeDBattpinsConfig(
        SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE
    );

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
 * @brief ADC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hadc->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA0     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        */
        GPIO_InitStruct.Pin  = VUSB_PWR_ADC_Pin | VUSB_PC_ADC_Pin | NTC_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

/**
 * @brief ADC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA0     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        */
        HAL_GPIO_DeInit(
            GPIOA, VUSB_PWR_ADC_Pin | VUSB_PC_ADC_Pin | NTC_ADC_Pin
        );

        /* ADC1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/**
 * @brief DAC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hdac->Instance == DAC1) {
        /* USER CODE BEGIN DAC1_MspInit 0 */

        /* USER CODE END DAC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_DAC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**DAC1 GPIO Configuration
        PA4     ------> DAC1_OUT1
        PA5     ------> DAC1_OUT2
        */
        GPIO_InitStruct.Pin  = MCU_ISET_DAC_Pin | MCU_ATRK_DAC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN DAC1_MspInit 1 */

        /* USER CODE END DAC1_MspInit 1 */
    }
}

/**
 * @brief DAC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hdac: DAC handle pointer
 * @retval None
 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
    if (hdac->Instance == DAC1) {
        /* USER CODE BEGIN DAC1_MspDeInit 0 */

        /* USER CODE END DAC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DAC1_CLK_DISABLE();

        /**DAC1 GPIO Configuration
        PA4     ------> DAC1_OUT1
        PA5     ------> DAC1_OUT2
        */
        HAL_GPIO_DeInit(GPIOA, MCU_ISET_DAC_Pin | MCU_ATRK_DAC_Pin);

        /* USER CODE BEGIN DAC1_MspDeInit 1 */

        /* USER CODE END DAC1_MspDeInit 1 */
    }
}

/**
 * @brief I2C MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef         GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit   = {0};
    if (hi2c->Instance == I2C1) {
        /* USER CODE BEGIN I2C1_MspInit 0 */

        /* USER CODE END I2C1_MspInit 0 */

        /** Initializes the peripherals clocks
         */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
        PeriphClkInit.I2c1ClockSelection   = RCC_I2C1CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PA9     ------> I2C1_SCL
        PA10     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin       = I2C1_SCL_Pin | I2C1_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        /* I2C1 interrupt Init */
        HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C1_IRQn);
        /* USER CODE BEGIN I2C1_MspInit 1 */

        /* USER CODE END I2C1_MspInit 1 */
    }
    else if (hi2c->Instance == I2C2) {
        /* USER CODE BEGIN I2C2_MspInit 0 */

        /* USER CODE END I2C2_MspInit 0 */

        /** Initializes the peripherals clocks
         */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
        PeriphClkInit.I2c2ClockSelection   = RCC_I2C2CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PA7     ------> I2C2_SCL
        PB4     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin       = I2C2_SCL_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_I2C2;
        HAL_GPIO_Init(I2C2_SCL_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = I2C2_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_I2C2;
        HAL_GPIO_Init(I2C2_SDA_GPIO_Port, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
        /* I2C2 interrupt Init */
        HAL_NVIC_SetPriority(I2C2_3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_3_IRQn);
        /* USER CODE BEGIN I2C2_MspInit 1 */

        /* USER CODE END I2C2_MspInit 1 */
    }
}

/**
 * @brief I2C MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == I2C1) {
        /* USER CODE BEGIN I2C1_MspDeInit 0 */

        /* USER CODE END I2C1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /**I2C1 GPIO Configuration
        PA9     ------> I2C1_SCL
        PA10     ------> I2C1_SDA
        */
        HAL_GPIO_DeInit(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin);

        HAL_GPIO_DeInit(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin);

        /* I2C1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C1_IRQn);
        /* USER CODE BEGIN I2C1_MspDeInit 1 */

        /* USER CODE END I2C1_MspDeInit 1 */
    }
    else if (hi2c->Instance == I2C2) {
        /* USER CODE BEGIN I2C2_MspDeInit 0 */

        /* USER CODE END I2C2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C2_CLK_DISABLE();

        /**I2C2 GPIO Configuration
        PA7     ------> I2C2_SCL
        PB4     ------> I2C2_SDA
        */
        HAL_GPIO_DeInit(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin);

        HAL_GPIO_DeInit(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin);

        /* I2C2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C2_3_IRQn);
        /* USER CODE BEGIN I2C2_MspDeInit 1 */

        /* USER CODE END I2C2_MspDeInit 1 */
    }
}

/**
 * @brief SPI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hspi->Instance == SPI1) {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA6     ------> SPI1_MISO
        PB3     ------> SPI1_SCK
        PB5     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin       = INA_SPI_MISO_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
        HAL_GPIO_Init(INA_SPI_MISO_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = INA_SPI_SCK_Pin | INA_SPI_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* SPI1 interrupt Init */
        HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }
}

/**
 * @brief SPI MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI1) {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA6     ------> SPI1_MISO
        PB3     ------> SPI1_SCK
        PB5     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(INA_SPI_MISO_GPIO_Port, INA_SPI_MISO_Pin);

        HAL_GPIO_DeInit(GPIOB, INA_SPI_SCK_Pin | INA_SPI_MOSI_Pin);

        /* SPI1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(SPI1_IRQn);
        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    if (htim_base->Instance == TIM4) {
        /* USER CODE BEGIN TIM4_MspInit 0 */

        /* USER CODE END TIM4_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM4_CLK_ENABLE();

        /* TIM4 DMA Init */
        /* TIM4_CH2 Init */
        hdma_tim4_ch2.Instance                 = DMA1_Channel1;
        hdma_tim4_ch2.Init.Request             = DMA_REQUEST_TIM4_CH2;
        hdma_tim4_ch2.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim4_ch2.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim4_ch2.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim4_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim4_ch2.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_tim4_ch2.Init.Mode                = DMA_CIRCULAR;
        hdma_tim4_ch2.Init.Priority            = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_tim4_ch2) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC2], hdma_tim4_ch2);

        /* USER CODE BEGIN TIM4_MspInit 1 */

        /* USER CODE END TIM4_MspInit 1 */
    }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (htim->Instance == TIM4) {
        /* USER CODE BEGIN TIM4_MspPostInit 0 */

        /* USER CODE END TIM4_MspPostInit 0 */

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM4 GPIO Configuration
        PB7     ------> TIM4_CH2
        */
        GPIO_InitStruct.Pin       = ADDR_LED_PWM_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM4;
        HAL_GPIO_Init(ADDR_LED_PWM_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM4_MspPostInit 1 */

        /* USER CODE END TIM4_MspPostInit 1 */
    }
}

/**
 * @brief TIM_Base MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    if (htim_base->Instance == TIM4) {
        /* USER CODE BEGIN TIM4_MspDeInit 0 */

        /* USER CODE END TIM4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM4_CLK_DISABLE();

        /* TIM4 DMA DeInit */
        HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC2]);
        /* USER CODE BEGIN TIM4_MspDeInit 1 */

        /* USER CODE END TIM4_MspDeInit 1 */
    }
}

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef         GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit   = {0};
    if (huart->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */

        /** Initializes the peripherals clocks
         */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PB0     ------> USART3_RX
        PB2     ------> USART3_TX
        */
        GPIO_InitStruct.Pin       = USART3_RX_Pin | USART3_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_4_5_6_LPUART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_4_5_6_LPUART1_IRQn);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }
}

/**
 * @brief UART MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PB0     ------> USART3_RX
        PB2     ------> USART3_TX
        */
        HAL_GPIO_DeInit(GPIOB, USART3_RX_Pin | USART3_TX_Pin);

        /* USART3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART3_4_5_6_LPUART1_IRQn);
        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }
}

/**
 * @brief PCD MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hpcd: PCD handle pointer
 * @retval None
 */
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (hpcd->Instance == USB_DRD_FS) {
        /* USER CODE BEGIN USB_DRD_FS_MspInit 0 */

        /* USER CODE END USB_DRD_FS_MspInit 0 */

        /** Initializes the peripherals clocks
         */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_USB_CLK_ENABLE();

        /* Enable VDDUSB */
        if (__HAL_RCC_PWR_IS_CLK_DISABLED()) {
            __HAL_RCC_PWR_CLK_ENABLE();
            HAL_PWREx_EnableVddUSB();
            __HAL_RCC_PWR_CLK_DISABLE();
        }
        else {
            HAL_PWREx_EnableVddUSB();
        }
        /* USB_DRD_FS interrupt Init */
        HAL_NVIC_SetPriority(USB_UCPD1_2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USB_UCPD1_2_IRQn);
        /* USER CODE BEGIN USB_DRD_FS_MspInit 1 */

        /* USER CODE END USB_DRD_FS_MspInit 1 */
    }
}

/**
 * @brief PCD MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hpcd: PCD handle pointer
 * @retval None
 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
    if (hpcd->Instance == USB_DRD_FS) {
        /* USER CODE BEGIN USB_DRD_FS_MspDeInit 0 */

        /* USER CODE END USB_DRD_FS_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USB_CLK_DISABLE();

        /* USB_DRD_FS interrupt DeInit */
        /* USER CODE BEGIN USB_DRD_FS:USB_UCPD1_2_IRQn disable */
        /**
         * Uncomment the line below to disable the "USB_UCPD1_2_IRQn" interrupt
         * Be aware, disabling shared interrupt may affect other IPs
         */
        /* HAL_NVIC_DisableIRQ(USB_UCPD1_2_IRQn); */
        /* USER CODE END USB_DRD_FS:USB_UCPD1_2_IRQn disable */

        /* USER CODE BEGIN USB_DRD_FS_MspDeInit 1 */

        /* USER CODE END USB_DRD_FS_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
