/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NAVIGATE_BTN_Pin GPIO_PIN_0
#define NAVIGATE_BTN_GPIO_Port GPIOA
#define SPEC_VIDEO_Pin GPIO_PIN_1
#define SPEC_VIDEO_GPIO_Port GPIOA
#define SPEC_EOS_Pin GPIO_PIN_4
#define SPEC_EOS_GPIO_Port GPIOA
#define StartTest_BTN_Pin GPIO_PIN_5
#define StartTest_BTN_GPIO_Port GPIOA
#define SPEC_LED_Pin GPIO_PIN_7
#define SPEC_LED_GPIO_Port GPIOA
#define BATT_LEVEL_Pin GPIO_PIN_0
#define BATT_LEVEL_GPIO_Port GPIOB
#define SPEC_CLK_Pin GPIO_PIN_9
#define SPEC_CLK_GPIO_Port GPIOE
#define ERR_BUZZER_Pin GPIO_PIN_12
#define ERR_BUZZER_GPIO_Port GPIOE
#define READY_LED_Pin GPIO_PIN_13
#define READY_LED_GPIO_Port GPIOE
#define NEXT_BTN_Pin GPIO_PIN_14
#define NEXT_BTN_GPIO_Port GPIOE
#define PREV_BTN_Pin GPIO_PIN_15
#define PREV_BTN_GPIO_Port GPIOE
#define IsCharging_EXTI_Pin GPIO_PIN_8
#define IsCharging_EXTI_GPIO_Port GPIOD
#define Bcode_INIT_Pin GPIO_PIN_11
#define Bcode_INIT_GPIO_Port GPIOD
#define BT_INIT_Pin GPIO_PIN_12
#define BT_INIT_GPIO_Port GPIOD
#define EN_5vReg_Pin GPIO_PIN_13
#define EN_5vReg_GPIO_Port GPIOD
#define RTC_SDA_Pin GPIO_PIN_9
#define RTC_SDA_GPIO_Port GPIOC
#define RTC_SCL_Pin GPIO_PIN_8
#define RTC_SCL_GPIO_Port GPIOA
#define LCD_CE_Pin GPIO_PIN_0
#define LCD_CE_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_1
#define LCD_DC_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_2
#define LCD_RST_GPIO_Port GPIOD
#define SPEC_START_Pin GPIO_PIN_5
#define SPEC_START_GPIO_Port GPIOB
#define SPEC_GAIN_Pin GPIO_PIN_8
#define SPEC_GAIN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
