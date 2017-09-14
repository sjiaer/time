/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define WKUP_Pin GPIO_PIN_0
#define WKUP_GPIO_Port GPIOA
#define e_Pin GPIO_PIN_1
#define e_GPIO_Port GPIOA
#define d_Pin GPIO_PIN_2
#define d_GPIO_Port GPIOA
#define c_Pin GPIO_PIN_3
#define c_GPIO_Port GPIOA
#define g_Pin GPIO_PIN_4
#define g_GPIO_Port GPIOA
#define a_Pin GPIO_PIN_5
#define a_GPIO_Port GPIOA
#define f_Pin GPIO_PIN_6
#define f_GPIO_Port GPIOA
#define b_Pin GPIO_PIN_7
#define b_GPIO_Port GPIOA
#define D5_D6_Pin GPIO_PIN_0
#define D5_D6_GPIO_Port GPIOB
#define D1G4_Pin GPIO_PIN_1
#define D1G4_GPIO_Port GPIOB
#define D1G1_Pin GPIO_PIN_2
#define D1G1_GPIO_Port GPIOB
#define D1G2_Pin GPIO_PIN_10
#define D1G2_GPIO_Port GPIOB
#define D1G3_Pin GPIO_PIN_11
#define D1G3_GPIO_Port GPIOB
#define RST1_Pin GPIO_PIN_12
#define RST1_GPIO_Port GPIOB
#define I_0_Pin GPIO_PIN_13
#define I_0_GPIO_Port GPIOB
#define SCLK1_Pin GPIO_PIN_14
#define SCLK1_GPIO_Port GPIOB
#define DQ_Pin GPIO_PIN_15
#define DQ_GPIO_Port GPIOB
#define REMOTE_Pin GPIO_PIN_8
#define REMOTE_GPIO_Port GPIOA
#define KEY0_Pin GPIO_PIN_9
#define KEY0_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_10
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_15
#define KEY2_GPIO_Port GPIOA
#define RS_Pin GPIO_PIN_3
#define RS_GPIO_Port GPIOB
#define SCLK2_Pin GPIO_PIN_4
#define SCLK2_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_5
#define CS_GPIO_Port GPIOB
#define RST2_Pin GPIO_PIN_6
#define RST2_GPIO_Port GPIOB
#define SDIN_Pin GPIO_PIN_7
#define SDIN_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
