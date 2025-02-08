/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "OLED.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void ESP_Init(void);
void ParseWeatherData(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t aRxBuffer;			//�����жϻ���
extern char RxBuffer[1000];
extern uint8_t dataReceived;
extern uint16_t Uart1_Rx_Cnt;
extern unsigned char BMP1[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
  OLED_Init();
  OLED_CLS();
  OLED_DrawBMP(80,3,128,8,BMP1);
  ESP_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  ParseWeatherData();
//	  HAL_Delay(5000);
	  
	  //printf("GET https://api.seniverse.com/v3/weather/now.json?key=Sx0WBVjlvmOfdCZeT&location=nanjing&language=en&unit=c\r\n");  //����WIFI
	//printf("GET https://api.seniverse.com/v3/weather/now.json?key=Sx0WBVjlvmOfdCZeT&location=nanjing&language=en&unit=c\r\n");  //����WIFI

    if (dataReceived)  // ȷ��ֻ���ڽ��յ���������ʱ�Ž��н���
    {
        //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
		ParseWeatherData();
        dataReceived = 0;  // ���������ݺ����ñ�־λ
		Uart1_Rx_Cnt = 0;
		memset(RxBuffer,0x00,sizeof(RxBuffer)); //�������
		//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
    }
	HAL_Delay(10000);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void ESP_Init(void)
{
	printf("ATE1\r\n");
	HAL_Delay(1000);
	printf("AT+CWJAP=\"BK\"\,\"123456789\"\r\n");  //����WIFI
	HAL_Delay(2000);
	printf("AT+CIPMODE=1\r\n");  //����WIFI
	HAL_Delay(1000);
	printf("AT+CIPSTART=\"TCP\"\,\"api.seniverse.com\"\,80\r\n");  //����WIFI
	HAL_Delay(1000);
	printf("AT+CIPSEND\r\n");  //����WIFI
	HAL_Delay(1000);  
	printf("GET https://api.seniverse.com/v3/weather/now.json?key=Sx0WBVjlvmOfdCZeT&location=beijing&language=en&unit=c\r\n");  //����WIFI
	HAL_Delay(2000);
	
//	ParseWeatherData();
}


void ParseWeatherData(void)
{
    char *jsonStart = strchr(RxBuffer, '{'); // ��λJSON��ʼλ�� strchr ����λ��ָ���ڴ��ַ
	
    if (jsonStart == NULL) {
        //printf("Error: JSON start not found\r\n");
        return;
    }

    // ����now����
    char *nowSection = strstr(jsonStart, "\"now\":{");
    if (nowSection == NULL) {
        //printf("Error: 'now' section not found\r\n");
        return;
    }

    // ��ȡ�¶�
    char *tempPtr = strstr(nowSection, "\"temperature\":\"");
    if (tempPtr) {
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
		tempPtr += strlen("\"temperature\":\"");
        char *tempEnd = strchr(tempPtr, '\"');
        if (tempEnd) {
            char temperature[10];
            strncpy(temperature, tempPtr, tempEnd - tempPtr);
            temperature[tempEnd - tempPtr] = '\0';
            //printf("Temperature: %s��C\r\n", temperature);
//			OLED_ShowNum(0,0,(uint8_t)temperature,3,16);
			OLED_ShowStr(0,0,temperature,2);
			OLED_ShowStr(20,0,"C",2);
            // ������ʾ���������� Display_Temperature(temperature);
        }
    }

    // ��ȡ����״��
    char *textPtr = strstr(nowSection, "\"text\":\"");
    if (textPtr) {
        textPtr += strlen("\"text\":\"");
        char *textEnd = strchr(textPtr, '\"');
        if (textEnd) {
            char weatherStatus[20];
            strncpy(weatherStatus, textPtr, textEnd - textPtr);
            weatherStatus[textEnd - textPtr] = '\0';
            //printf("Weather: %s\r\n", weatherStatus);
			//OLED_ShowNum(0,5,(uint8_t)weatherStatus,3,16);
			OLED_ShowStr(0,5,weatherStatus,2);
            // ������ʾ���������� Display_Weather(weatherStatus);
        }
    }
	
	// ��ȡ�ص�״��
    char *locationPtr = strstr(jsonStart, "\"name\":\"");
    if (locationPtr) {
        locationPtr += strlen("\"name\":\"");
        char *locationEnd = strchr(locationPtr, '\"');
        if (locationEnd) {
            char location[30];
            strncpy(location, locationPtr, locationEnd - locationPtr);
            location[locationEnd - locationPtr] = '\0';   //ʹ���Ϊ�ַ���
            //printf("Weather: %s\r\n", weatherStatus);
			//OLED_ShowNum(0,5,(uint8_t)weatherStatus,3,16);
			OLED_ShowStr(0,3,location,2);
            // ������ʾ���������� Display_Weather(weatherStatus);
        }
    }
	
	char *timePtr = strstr(jsonStart, "\"last_update\":\"");
    if (timePtr) {
        timePtr += strlen("\"last_update\":\"");
        char *timeEnd = strchr(timePtr, '\"');
        if (timeEnd) {
            char lastUpdate[30];
            strncpy(lastUpdate, timePtr, 10);
            lastUpdate[timeEnd - timePtr] = '\0';
            //printf("Last Update: %s\r\n", lastUpdate);
            OLED_ShowStr(40,0,lastUpdate,2); // ��ʾʱ��
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
