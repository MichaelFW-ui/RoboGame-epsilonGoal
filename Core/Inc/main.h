/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOE
#define HandspikeA_Pin GPIO_PIN_6
#define HandspikeA_GPIO_Port GPIOE
#define HandspikeB_Pin GPIO_PIN_13
#define HandspikeB_GPIO_Port GPIOC
#define Motor_OutAP_Pin GPIO_PIN_0
#define Motor_OutAP_GPIO_Port GPIOF
#define Motor_OutAN_Pin GPIO_PIN_1
#define Motor_OutAN_GPIO_Port GPIOF
#define Motor_OutBP_Pin GPIO_PIN_2
#define Motor_OutBP_GPIO_Port GPIOF
#define Motor_OutBN_Pin GPIO_PIN_3
#define Motor_OutBN_GPIO_Port GPIOF
#define Motor_OutCP_Pin GPIO_PIN_4
#define Motor_OutCP_GPIO_Port GPIOF
#define Motor_OutCN_Pin GPIO_PIN_5
#define Motor_OutCN_GPIO_Port GPIOF
#define Pushrod_Pulse_Pin GPIO_PIN_6
#define Pushrod_Pulse_GPIO_Port GPIOF
#define Pushrod_Direction_Pin GPIO_PIN_8
#define Pushrod_Direction_GPIO_Port GPIOF
#define Motor_A_Pin GPIO_PIN_0
#define Motor_A_GPIO_Port GPIOA
#define Motor_B_Pin GPIO_PIN_1
#define Motor_B_GPIO_Port GPIOA
#define Motor_C_Pin GPIO_PIN_2
#define Motor_C_GPIO_Port GPIOA
#define Motor_D_Pin GPIO_PIN_3
#define Motor_D_GPIO_Port GPIOA
#define Motor_InA_Pin GPIO_PIN_2
#define Motor_InA_GPIO_Port GPIOB
#define Motor_OutDP_Pin GPIO_PIN_11
#define Motor_OutDP_GPIO_Port GPIOF
#define Motor_OutDN_Pin GPIO_PIN_12
#define Motor_OutDN_GPIO_Port GPIOF
#define Motor_InB_Pin GPIO_PIN_13
#define Motor_InB_GPIO_Port GPIOF
#define Motor_InC_Pin GPIO_PIN_14
#define Motor_InC_GPIO_Port GPIOF
#define Motor_InD_Pin GPIO_PIN_0
#define Motor_InD_GPIO_Port GPIOG
#define PWM_SCL_Pin GPIO_PIN_10
#define PWM_SCL_GPIO_Port GPIOB
#define PWM_SDA_Pin GPIO_PIN_11
#define PWM_SDA_GPIO_Port GPIOB
#define GY_CS_Pin GPIO_PIN_3
#define GY_CS_GPIO_Port GPIOG
#define VA_CS_Pin GPIO_PIN_4
#define VA_CS_GPIO_Port GPIOG
#define VB_CS_Pin GPIO_PIN_5
#define VB_CS_GPIO_Port GPIOG
#define NRF_CE_Pin GPIO_PIN_6
#define NRF_CE_GPIO_Port GPIOG
#define NRF_CS_Pin GPIO_PIN_7
#define NRF_CS_GPIO_Port GPIOG
#define NRF_IRQ_Pin GPIO_PIN_8
#define NRF_IRQ_GPIO_Port GPIOG
#define POWER_DETECT_Pin GPIO_PIN_9
#define POWER_DETECT_GPIO_Port GPIOC
#define Debug_TX_Pin GPIO_PIN_10
#define Debug_TX_GPIO_Port GPIOC
#define Debug_RX_Pin GPIO_PIN_11
#define Debug_RX_GPIO_Port GPIOC
#define Connection_TX_Pin GPIO_PIN_12
#define Connection_TX_GPIO_Port GPIOC
#define Connection_RX_Pin GPIO_PIN_2
#define Connection_RX_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
