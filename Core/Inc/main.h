/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f2xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define tiem_can1_Pin GPIO_PIN_0
#define tiem_can1_GPIO_Port GPIOC
#define tiem_can2_Pin GPIO_PIN_1
#define tiem_can2_GPIO_Port GPIOC
#define tiem_can3_Pin GPIO_PIN_2
#define tiem_can3_GPIO_Port GPIOC
#define pressure_sensor_Pin GPIO_PIN_0
#define pressure_sensor_GPIO_Port GPIOA
#define VALVE_CTL_00_Pin GPIO_PIN_1
#define VALVE_CTL_00_GPIO_Port GPIOA
#define VALVE_CTL_01_Pin GPIO_PIN_2
#define VALVE_CTL_01_GPIO_Port GPIOA
#define LED_CTL_00_Pin GPIO_PIN_3
#define LED_CTL_00_GPIO_Port GPIOA
#define LED_CTL_01_Pin GPIO_PIN_4
#define LED_CTL_01_GPIO_Port GPIOA
#define LED_CTL_02_Pin GPIO_PIN_5
#define LED_CTL_02_GPIO_Port GPIOA
#define LED_CTL_03_Pin GPIO_PIN_6
#define LED_CTL_03_GPIO_Port GPIOA
#define air_compressor_Pin GPIO_PIN_7
#define air_compressor_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
