/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define DRW_GATE_CLOSE_Pin GPIO_PIN_13
#define DRW_GATE_CLOSE_GPIO_Port GPIOC
#define DRW_GATE_CLOSE_EXTI_IRQn EXTI15_10_IRQn
#define OUT_LAMP_PATH_Pin GPIO_PIN_0
#define OUT_LAMP_PATH_GPIO_Port GPIOD
#define OUT_G_GATE_Pin GPIO_PIN_1
#define OUT_G_GATE_GPIO_Port GPIOD
#define LIV_WIN_STATUS_Pin GPIO_PIN_0
#define LIV_WIN_STATUS_GPIO_Port GPIOC
#define LIV_WIN_STATUS_EXTI_IRQn EXTI0_IRQn
#define LIV_MOVE_Pin GPIO_PIN_1
#define LIV_MOVE_GPIO_Port GPIOC
#define LIV_MOVE_EXTI_IRQn EXTI1_IRQn
#define BED_MOVE_Pin GPIO_PIN_2
#define BED_MOVE_GPIO_Port GPIOC
#define BED_MOVE_EXTI_IRQn EXTI2_IRQn
#define RAIN_Pin GPIO_PIN_3
#define RAIN_GPIO_Port GPIOC
#define WATERING_Pin GPIO_PIN_0
#define WATERING_GPIO_Port GPIOA
#define BED_WIN_Pin GPIO_PIN_1
#define BED_WIN_GPIO_Port GPIOA
#define GAR_LAMP_Pin GPIO_PIN_2
#define GAR_LAMP_GPIO_Port GPIOA
#define OUT_LAMP_Pin GPIO_PIN_3
#define OUT_LAMP_GPIO_Port GPIOA
#define LIV_WIN_Pin GPIO_PIN_4
#define LIV_WIN_GPIO_Port GPIOA
#define BED_LAMP_2_Pin GPIO_PIN_5
#define BED_LAMP_2_GPIO_Port GPIOA
#define ADC_SUN_Pin GPIO_PIN_6
#define ADC_SUN_GPIO_Port GPIOA
#define BED_LAMP_1_Pin GPIO_PIN_7
#define BED_LAMP_1_GPIO_Port GPIOA
#define LIV_LAMP_STATUS_Pin GPIO_PIN_4
#define LIV_LAMP_STATUS_GPIO_Port GPIOC
#define LIV_LAMP_STATUS_EXTI_IRQn EXTI4_IRQn
#define BED_LAMP_STATUS_Pin GPIO_PIN_5
#define BED_LAMP_STATUS_GPIO_Port GPIOC
#define BED_LAMP_STATUS_EXTI_IRQn EXTI9_5_IRQn
#define LIV_LAMP_2_Pin GPIO_PIN_0
#define LIV_LAMP_2_GPIO_Port GPIOB
#define PUMP_WATER_Pin GPIO_PIN_1
#define PUMP_WATER_GPIO_Port GPIOB
#define HEATER_WATER_Pin GPIO_PIN_2
#define HEATER_WATER_GPIO_Port GPIOB
#define LIV_LAMP_1_Pin GPIO_PIN_12
#define LIV_LAMP_1_GPIO_Port GPIOB
#define G_GATE_Pin GPIO_PIN_13
#define G_GATE_GPIO_Port GPIOB
#define G_GATE_CLOSE_Pin GPIO_PIN_14
#define G_GATE_CLOSE_GPIO_Port GPIOB
#define G_GATE_CLOSE_EXTI_IRQn EXTI15_10_IRQn
#define BED_WIN_STATUS_Pin GPIO_PIN_15
#define BED_WIN_STATUS_GPIO_Port GPIOB
#define BED_WIN_STATUS_EXTI_IRQn EXTI15_10_IRQn
#define DRW_GATE_Pin GPIO_PIN_6
#define DRW_GATE_GPIO_Port GPIOC
#define BED_FAN_2_Pin GPIO_PIN_7
#define BED_FAN_2_GPIO_Port GPIOC
#define BED_FAN_1_Pin GPIO_PIN_8
#define BED_FAN_1_GPIO_Port GPIOC
#define LIV_FAN_2_Pin GPIO_PIN_9
#define LIV_FAN_2_GPIO_Port GPIOC
#define LIV_FAN_1_Pin GPIO_PIN_8
#define LIV_FAN_1_GPIO_Port GPIOA
#define DRW_GATE_OPEN_Pin GPIO_PIN_10
#define DRW_GATE_OPEN_GPIO_Port GPIOA
#define DRW_GATE_OPEN_EXTI_IRQn EXTI15_10_IRQn
#define BED_BLIND_2_Pin GPIO_PIN_11
#define BED_BLIND_2_GPIO_Port GPIOA
#define G_GATE_OPEN_Pin GPIO_PIN_12
#define G_GATE_OPEN_GPIO_Port GPIOA
#define G_GATE_OPEN_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define BED_BLIND_1_Pin GPIO_PIN_10
#define BED_BLIND_1_GPIO_Port GPIOC
#define SENSOR_Pin GPIO_PIN_11
#define SENSOR_GPIO_Port GPIOC
#define SENSOR_EXTI_IRQn EXTI15_10_IRQn
#define LIV_BLIND_2_Pin GPIO_PIN_12
#define LIV_BLIND_2_GPIO_Port GPIOC
#define LIV_BLIND_1_Pin GPIO_PIN_2
#define LIV_BLIND_1_GPIO_Port GPIOD
#define BED_HEAT_Pin GPIO_PIN_4
#define BED_HEAT_GPIO_Port GPIOB
#define LIV_HEAT_Pin GPIO_PIN_5
#define LIV_HEAT_GPIO_Port GPIOB
#define PUMP_CH_Pin GPIO_PIN_6
#define PUMP_CH_GPIO_Port GPIOB
#define HEATER_CH_Pin GPIO_PIN_7
#define HEATER_CH_GPIO_Port GPIOB
#define DHT12_Pin GPIO_PIN_8
#define DHT12_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_9
#define DHT11_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
