/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "cJSON.H"
#include "math.h"
#include "LibUser.h"
#include "SimpleKalmanFilter.h"
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
SimpleKalmanFilter kf = SimpleKalmanFilter(2, 2, 0.01);
int chooseMain = 0;
float standardPressure = 800;
/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
float adcKalman(float adc){
	return kf.updateEstimate(adc);
}
//------uart esp32-------//
uint8_t uart;
uint8_t uart_data[10];
uint8_t uart_index = 0;
uint8_t uart_end   = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	UNUSED(huart);
	if(huart -> Instance == USART1)
	{
		if(uart != 13)
		{
			uart_data[uart_index++] = uart;
		}
		else if(uart == 13)// truyen xong du lieu
		{
			uart_data[uart_index++]='\0';
			uart_index = 0;
			uart_end =1;
		}
	}
	HAL_UART_Receive_IT(&huart1,&uart,1);
}
void uart_handle(){
	if(uart_end == 1){
		uint8_t argv[10][20];
		uint8_t arg_num =0;
		char * pch;
		pch = strtok((char *)uart_data," ");
		while(pch != NULL){
			strcpy((char *) argv[arg_num++],pch);
			pch =  strtok(NULL," ");
		}
		uart_end=0;
		if(strcmp((char *)argv[1],"1")==0){
			chooseMain = 1 ;
		}
		else if(strcmp((char *)argv[1],"2")==0){
			chooseMain =2;
		}
		else if(strcmp((char *)argv[1],"3")==0){
			chooseMain =3;
		}
		else if(strcmp((char *)argv[1],"4")==0){
			chooseMain =4;
		}
	}
}
//----------modeTest-------------//
uint32_t startTime;
void ledTest(){
	  static int choose;
	  static uint8_t countled, countOneLed;
	  switch(choose){
			case 0:
				if(HAL_GetTick()- startTime > 3000){
				HAL_GPIO_TogglePin(LED_CTL_00_GPIO_Port,LED_CTL_00_Pin);
				HAL_GPIO_TogglePin(LED_CTL_00_GPIO_Port,LED_CTL_01_Pin);
				HAL_GPIO_TogglePin(LED_CTL_00_GPIO_Port,LED_CTL_02_Pin);
				HAL_GPIO_TogglePin(LED_CTL_00_GPIO_Port,LED_CTL_03_Pin);
				startTime=HAL_GetTick();
				countled++;
					if(countled>=3)
					{
						countled = 0;
						choose = 1;
						countOneLed = 0;
					}
		    }
		   break;
			case 1:
				if(HAL_GetTick()- startTime > 1000){
					countOneLed++;
					if(countOneLed >= 4)countOneLed =0;
					startTime = HAL_GetTick();
					ledSign(countOneLed);
					countled++;
					if(countled >= 16)
					{
						ledSign(4);
						countled =0;
						choose = 0;
					}
				}
				break;
	}
}
void proximityTest(){
	
	uint8_t dich_trai, gia_tri;
    dich_trai = 0;
	  gia_tri=0;
		gia_tri   = px_left;
	  dich_trai = dich_trai |(gia_tri<< 2);
	  gia_tri   = px_right;
	  dich_trai = dich_trai |(gia_tri<<1);
	  gia_tri   = px_center;
	  dich_trai = dich_trai | gia_tri;
	  switch(dich_trai)
		{
			case 1:
			{
				dich_trai = 2;// dám thang
			  ledSign(2);
			}
			break;
			case 2:
			{
				dich_trai = 1;// punch left
			ledSign(0);
			}
			break;
			case 3:
			{
			 dich_trai = 1; // punch left
			 ledSign(0);
			}
			break;
			case 4:
			{
				dich_trai = 3;// punch right
			  ledSign(1);
			}
			case 5:
			{
				dich_trai = 3;// punch right
			  ledSign(1);
			}
			break;
			case 6:
			{
				dich_trai = 4;// pucnh tall
				ledSign(3);
			}
			break;
			case 7:
			{
			dich_trai = 4;// moc len
			ledSign(3);
			}
			break;
			default:
			{
				dich_trai = 0;
				 ledSign(4);
			}
				break;
		}
}
void dataSendEsp32(uint8_t punch_position,float adc){
		  char data_adc[5],data_position[5];
			char pData[]= "{\"luc\":";
			char dau_ngoac[]="}";
			char dataSend[30];
			sprintf(data_adc,"%0.2f",adc);
			sprintf(data_position,"%d",punch_position);
			strcpy(dataSend,pData);
			strcat(dataSend,data_adc);
			strcat(dataSend,",\"position\":");
			strcat(dataSend,data_position);
			strcat(dataSend,dau_ngoac);
			strcat(dataSend,"\n");
			HAL_UART_Transmit(&huart1, (uint8_t *)dataSend,strlen((char *)dataSend),10);
}
//---------------Read adc--------------//
uint32_t adc_value; // giá tri adc
float max_adc_value;// giá tri max adc;
int dem_punch1 =0;
typedef enum{False,True}boolean;
typedef struct{
	uint32_t froce;
	uint8_t position;
	boolean havePunch;
}Punch;
Punch punch;
void evenPuch(){
  //static uint32_t maxAdc;
	static int count=0;
	uint32_t adckalman = adcKalman(adc_value);
	float limit = standardPressure+standardPressure*0.1;
	if(adckalman > limit && position()!=0){
		if(punch.froce < adckalman)
		{
			punch.froce = adckalman;
		}
		else{
			count++;
		}
		if(count > 10)
		{
			punch.havePunch = True;
			punch.position = position();
		}
	}
	if(punch.havePunch == True){
		dataSendEsp32(punch.position,punch.froce);
		punch.havePunch = False;
		punch.froce = 0;
	}
}
//-----------dataSend-v1--------------------//
void data_send(float adc, uint8_t position){
static int gia_tri_tiem_can,dem_punch;// dem_punch bat suon xuong
static uint8_t punch_position;
	if(position!= 0 && adc_value >(standardPressure +standardPressure*0.1))
		{
			dem_punch=0;
			gia_tri_tiem_can = 1;
			if(adc > max_adc_value)
    	{
				max_adc_value = adc;
			}
			punch_position = position;
		}
		else
		{
		dem_punch++;
		if(gia_tri_tiem_can == 1 && dem_punch == 5000)
		{
		  dataSendEsp32(punch_position,max_adc_value);
			max_adc_value = 0;
			gia_tri_tiem_can = 0;
			if(dem_punch1 ==50)dem_punch=0;
			dem_punch1++;
		}
  }
}

//---------aircompress-------------//
void airCompressTest(){
	static int count;
	static int upperlimit;
	if(HAL_GetTick()-startTime > 10000){
		count++;
		if(count ==1){
			upperlimit = 1200;
			standardPressure = 1200;
		}
		else if(count == 2)
		{
			upperlimit = 800;
			standardPressure = 800;
			HAL_GPIO_WritePin(VALVE_CTL_00_GPIO_Port,VALVE_CTL_00_Pin,GPIO_PIN_SET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(VALVE_CTL_00_GPIO_Port,VALVE_CTL_00_Pin,GPIO_PIN_RESET);
			count =0;
		}
		if(count > 3)
		{
			count =0;
		}
		startTime = HAL_GetTick();
	}
	airConditioner(adc_value,upperlimit);
}
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1,&uart,1);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&adc_value,1);
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
		//ledTest();
	 uart_handle();
	 switch(chooseMain){
		 case 1:
			ledTest();
		 break;
		 case 2:
			proximityTest();
			 break;
		 case 3:
			airCompressTest();
			 break;
		 case 4:
			 //data_send(adc_value,(uint8_t)position());
		   evenPuch();
			 break;
		 default: 
			 evenPuch();
			 break;
	  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

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
