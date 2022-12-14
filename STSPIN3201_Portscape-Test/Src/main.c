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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM14_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_NVIC_Init(void);
/*-- My code --*/
void Home_Offset(void);
void Mode_Home(void);
void Mode_Home2(void);
void Mode_Home3(void);
void Home_Null_Offset(void);

void OSC_MODE_HOME(void);
void OSC_MODE_HOME2(void);
void MainWhile_Homing(void);
/*-- My code ends --*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*-- My code --*/
   bool stepA=1,stepB=0,stepC=0,stepCA=0,stepAB=0,stepBC=0,pendingPhase=1, moveMotor =1;
   uint16_t oldhallstate =0, presenthallstate=0,count =0,oldAngle=0,execute = 0,waitToMoveCounter =0;
   int16_t pulse = 100;
   uint8_t MotorHallState = 0;
   uint8_t buffer[20];
//   uint8_t homingOffset = 0;

   uint16_t Null_Offset = 0;
/*-- My code ends --*/
#define STATE_0 (uint8_t)0
#define STATE_1 (uint8_t)1
#define STATE_2 (uint8_t)2
#define STATE_3 (uint8_t)3
#define STATE_4 (uint8_t)4
#define STATE_5 (uint8_t)5
#define STATE_6 (uint8_t)6
#define STATE_7 (uint8_t)7
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
  MX_ADC_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_MotorControl_Init();


    /* Initialize interrupts */
    MX_NVIC_Init();
    /* USER CODE BEGIN 2 */

    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);

    /* USER CODE END 2 */
    //MotorHallState = HALL_M1.HallStateCounter;
   // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t STATE: %u\r\n", HALL_M1.HallStateCounter), 100);
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
//    	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HALL_M1.HallState), 100);
//    	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallStateCounter:  %u\r\n", HALL_M1.HallStateCounter), 100);
//    	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallStateHomePosition:  %u\r\n", HALL_M1.HallStateHomePosition), 100);
//    	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounterHomePosition:  %u\r\n", HALL_M1.HallCounterHomePosition), 100);
    	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounterHomePosition:  %u\r\n", Null_Offset), 100);
    	HAL_Delay(1000);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


void Mode_Home(){

	uint8_t HallState_Internal = HALL_M1.HallState;
	uint8_t HallStateCounter_Internal = (HALL_M1.HallStateCounter + (Null_Offset%72));
	uint8_t HallState_Home = HALL_M1.HallStateHomePosition;
	uint8_t HallStateCounter_Home = HALL_M1.HallCounterHomePosition;

//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Input:  %u\r\n", HallState_Internal), 100);
	while (HallStateCounter_Internal != HallStateCounter_Home){ //&&(HallState_Home != HallState_Home)
		switch(HallState_Internal){
		//step 1
				case STATE_6:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 2
				case STATE_1:
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 3
				case STATE_2:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 4
				case STATE_3:
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
						  HallState_Internal++;
						  HallStateCounter_Internal++;
						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallState:  %u\r\n", HallState_Internal), 100);
//						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 5
				case STATE_4:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 6
				case STATE_5:
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				default:
					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallState:  %u\r\n", HallState_Internal), 100);
//					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
		}

		HallStateCounter_Internal = HallStateCounter_Internal%72;

		if(HallState_Internal ==7)
		{
			HallState_Internal =1;
		}

	}
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  //NOte this two lines
//	  HALL_M1.HallStateCounter = 0;
	  HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
	  HALL_M1.HallStateHomePosition = HALL_M1.HallState;
	  Null_Offset = 0;
}

//void Home_Offset(){
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "---------------------------------------\n"), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Prev STATE COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t PRev Hall POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
//	HALL_M1.HallCounterHomePosition = HALL_M1.HallStateCounter +1;
////	homingOffset = HALL_M1.HallStateCounter +1;
////	homingOffset +=1;
////	homingOffset = homingOffset%72;
//	//HALL_M1.HallStateHomePosition = HALL_M1.HallState;
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t New STATE COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t New Hall POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
//	Mode_Home2();
////	HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "---------------------------------------\n"), 100);
//}

void Home_Offset(){
	// 21 01 10 34
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Prev HALL homrCOunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t PRev Hall State home POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
	HALL_M1.HallCounterHomePosition = HALL_M1.HallCounterHomePosition +1;
//	HALL_M1.HallStateHomePosition = HALL_M1.HallState;
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t New HAll home COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t New Hall home State POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
	Mode_Home3();
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Final HAll Home COunter: %u\r\n", HALL_M1.HallCounterHomePosition), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Final Hall state home POSITION: %u\r\n", HALL_M1.HallStateHomePosition), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Final hall COunter: %u\r\n", HALL_M1.HallStateCounter), 100);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Final Hall POSITION: %u\r\n", HALL_M1.HallState), 100);
}
void Mode_Home3(){

	uint8_t HallState_Internal = HALL_M1.HallState;
	uint8_t HallStateCounter_Internal = HALL_M1.HallStateCounter;
	uint8_t HallState_Home = HALL_M1.HallStateHomePosition;
	uint8_t HallStateCounter_Home = HALL_M1.HallCounterHomePosition;

	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Input:  %u\r\n", HallState_Internal), 100);
//	while (HallStateCounter_Internal != HallStateCounter_Home){ //&&(HallState_Home != HallState_Home)
		switch(HallState_Internal){
		//step 1
				case STATE_6:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 2
				case STATE_1:
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 3
				case STATE_2:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 4
				case STATE_3:
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
						  HallState_Internal++;
						  HallStateCounter_Internal++;
						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallState:  %u\r\n", HallState_Internal), 100);
//						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 5
				case STATE_4:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				//step 6
				case STATE_5:
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
				default:
					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallState:  %u\r\n", HallState_Internal), 100);
//					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
					break;
		}

		HallStateCounter_Internal = HallStateCounter_Internal%72;

		if(HallState_Internal ==7)
		{
			HallState_Internal =1;
		}

//	}
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  //NOte this two lines
//	  HALL_M1.HallStateCounter = 0;
	  HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
	  HALL_M1.HallStateHomePosition = HALL_M1.HallState;
}
void Mode_Home2(){
	uint8_t HallState_Internal = HALL_M1.HallState;
	uint8_t HallStateCounter_Internal = HALL_M1.HallStateCounter;
	uint8_t HallState_Home = HALL_M1.HallStateHomePosition;
	uint8_t HallStateCounter_Home = HALL_M1.HallCounterHomePosition;// + homingOffset;

	while (HallStateCounter_Internal != HallStateCounter_Home){ //&&(HallState_Home != HallState_Home)
		switch(HallState_Internal){
			   //step 1
			   case STATE_6://STATE_2:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 2
				case STATE_1://STATE_3:
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 3
				case STATE_2://STATE_4:
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
					  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 1, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 4
				case STATE_3://STATE_5:
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
						  HallState_Internal++;
						  HallStateCounter_Internal++;
						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 3, HallState:  %u\r\n", HallState_Internal), 100);
//						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 6, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//						  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//					      for(int i =0; i<5000;i++);
					break;
				//step 5
				case STATE_4://STATE_6:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 4, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				//step 6
				case STATE_5://STATE_1:
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					  HallState_Internal++;
					  HallStateCounter_Internal++;
					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallState:  %u\r\n", HallState_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 2, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//				      for(int i =0; i<5000;i++);
					break;
				default:
					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallState:  %u\r\n", HallState_Internal), 100);
//					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t Error, HallCounter:  %u\r\n", HallStateCounter_Internal), 100);
//					HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t CASE 5, HallStateCounter_Home:  %u\r\n", HallStateCounter_Home), 100);
//					for(int i =0; i<5000;i++);
					break;
		}

		HallStateCounter_Internal = HallStateCounter_Internal%72;
		if(HallState_Internal ==7)
		{
			HallState_Internal =1;
		}

	}
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  HALL_M1.HallStateCounter = HALL_M1.HallCounterHomePosition;
	  HALL_M1.HallStateHomePosition = HALL_M1.HallState;
}

void Home_Null_Offset(){
//	Null_Offset = 0;
	Null_Offset = Null_Offset + HALL_M1.HallStateCounter;
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_BRK_UP_TRG_COM_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* EXTI0_1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_LEFT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_TRGO;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = ((TIM_CLOCK_DIVIDER) - 1);
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = ((PWM_PERIOD_CYCLES) / 2);
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim1.Init.RepetitionCounter = (REP_COUNTER);
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC4REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = (((PWM_PERIOD_CYCLES) / 2) - (HTMIN));
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_1;
  sBreakDeadTimeConfig.DeadTime = ((DEAD_TIME_COUNTS) / 2);
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_HallSensor_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = M1_HALL_TIM_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = M1_HALL_IC_FILTER;
  sConfig.Commutation_Delay = 0;
  if (HAL_TIMEx_HallSensor_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC2REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 0;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 0x800;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0x400;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */
  HAL_TIM_MspPostInit(&htim14);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OC_SEL_GPIO_Port, OC_SEL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(_3FG_HIZ_GPIO_Port, _3FG_HIZ_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OCTH_STBY2_GPIO_Port, OCTH_STBY2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OCTH_STBY1_GPIO_Port, OCTH_STBY1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : Start_Stop_Pin */
  GPIO_InitStruct.Pin = Start_Stop_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Start_Stop_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OC_SEL_Pin _3FG_HIZ_Pin */
  GPIO_InitStruct.Pin = OC_SEL_Pin|_3FG_HIZ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OCTH_STBY2_Pin OCTH_STBY1_Pin */
  GPIO_InitStruct.Pin = OCTH_STBY2_Pin|OCTH_STBY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

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


void MainWhile_Homing(){
	  /*--My code--*/
	  /*--My code ends --*/
    /* USER CODE END WHILE */
//	  oldAngle =(uint16_t *) HALL_M1.HallState;

/* 	  	  if( HALL_M1.HallStateCounter != 0 && moveMotor) //Default Position 0
	  	  {

	  		  if(stepA && pendingPhase)
	  		  {
	  //			  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "in phase control\n"), 100);
	  			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
	  			    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  			  stepA =0 ;
	  			  stepAB=1;
	  			  pendingPhase = 0;
	  			  execute++;
	  		  }
	  		  if(stepAB && pendingPhase)
	  		  {
	  //			  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "in phase control\n"), 100);
	  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
	  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
	  		  	   stepAB =0 ;
	  		  	   stepB=1;
	  		  	   pendingPhase = 0;
	  		  	}
	  		  if(stepB && pendingPhase)
	  		  	{
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
				  stepB = 0;
				  stepBC =1;
				  pendingPhase = 0;
	  		  	}
	  		  if(stepBC && pendingPhase)
	  		  	{
	  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
	  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
	  		  	    stepBC = 0;
	  		  	    stepC =1;
	  		  	    pendingPhase = 0;
	  		  	 }
	  	      if(stepC && pendingPhase)
	  	      	 {
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					stepC = 0;
					stepCA=1;
					pendingPhase = 0;
	  	         }
	  	      if(stepCA && pendingPhase)
	  	      	 {
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					stepC = 0;
					stepA=1;
					pendingPhase = 0;
	  	      	  }

	  	        pendingPhase = 1;

	  	  }
	  	  else
	  	  {
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
				  moveMotor = 0;
				  if(count == 0 )
				  {
						   oldhallstate =(uint16_t *) HALL_M1.HallState;
						   count =1;
				  }
					  execute=0;
					  HAL_Delay(0.1);

	  	  }

	  	  presenthallstate = (uint16_t *) HALL_M1.HallState;

		  if( HALL_M1.HallState == 71 && (oldhallstate != presenthallstate))
					  {
						  if( oldhallstate < presenthallstate)
						  {
							  HALL_M1.HallStateCounter = HALL_M1.HallStateCounter-oldhallstate+1;
							 // execute = execute - oldhallstate;
						  }
						  else
						  {
							  HALL_M1.HallStateCounter = HALL_M1.HallStateCounter-presenthallstate-1;
							 // execute = execute - presenthallstate;
						  }

						 count =0;
						 execute=0;
					  }
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\n at pole: %u\r\n", HALL_M1.HallStateCounter), 75);

		  execute = execute%7;
		 // HAL_Delay(10);
		  waitToMoveCounter++;
		  if(moveMotor == 0 && waitToMoveCounter >=50)
		  {
			  moveMotor =1;
			  waitToMoveCounter=0;
		  }*/
}
void OSC_MODE_HOME2(){

	  if( HALL_M1.HallStateCounter != 0 && moveMotor) //Default Position 0
  	  {

  		  if(stepA && pendingPhase)
  		  {
  //			  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "in phase control\n"), 100);
  			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
  			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
  			    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
  			  stepA =0 ;
  			  stepAB=1;
  			  pendingPhase = 0;
  			  execute++;
  		  }
  		  if(stepAB && pendingPhase)
  		  {
  //			  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "in phase control\n"), 100);
  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
  		  	   stepAB =0 ;
  		  	   stepB=1;
  		  	   pendingPhase = 0;
  		  	}
  		  if(stepB && pendingPhase)
  		  	{
			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			  stepB = 0;
			  stepBC =1;
			  pendingPhase = 0;
  		  	}
  		  if(stepBC && pendingPhase)
  		  	{
  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
  		  	    stepBC = 0;
  		  	    stepC =1;
  		  	    pendingPhase = 0;
  		  	 }
  	      if(stepC && pendingPhase)
  	      	 {
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
				stepC = 0;
				stepCA=1;
				pendingPhase = 0;
  	         }
  	      if(stepCA && pendingPhase)
  	      	 {
				 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
				 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
				stepC = 0;
				stepA=1;
				pendingPhase = 0;
  	      	  }

  	        pendingPhase = 1;

  	  }
  	  else
  	  {
			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
			  moveMotor = 0;
			  if(count == 0 )
			  {
					//   oldhallstate =(uint16_t *) HALL_M1.HallState;
					   count =1;
			  }
				  execute=0;
				  HAL_Delay(0.1);

  	  }

  	 // presenthallstate = (uint16_t *) HALL_M1.HallState;

	  if( HALL_M1.HallState == 71 && (HALL_M1.HallStateHomePosition != HALL_M1.HallState))
				  {
					  if( oldhallstate < HALL_M1.HallState)
					  {
						  HALL_M1.HallStateCounter = HALL_M1.HallStateCounter-HALL_M1.HallStateHomePosition+1;
						 // execute = execute - oldhallstate;
					  }
					  else
					  {
						  HALL_M1.HallStateCounter = HALL_M1.HallStateCounter-HALL_M1.HallState-1;
						 // execute = execute - presenthallstate;
					  }

					 count =0;
					 execute=0;
				  }
	  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\n at pole: %u\r\n", HALL_M1.HallStateCounter), 75);

	  execute = execute%7;
	 // HAL_Delay(10);
	  waitToMoveCounter++;
	  if(moveMotor == 0 && waitToMoveCounter >=50)
	  {
		  moveMotor =1;
		  waitToMoveCounter=0;
	  }
}

/*-- My code --*/
void OSC_MODE_HOME(){

//	while(HALL_M1.HallStateCounter != 0){
//		 HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t HOMING COunter: %u\r\n", HALL_M1.HallStateCounter), 100);
//	moveMotor = 1;
	int diff = HALL_M1.HallStateCounter - HALL_M1.HallStateHomePosition;
	for(int i = 0; i<=diff;){
//			 HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t DO COunter: %u\r\n", HALL_M1.HallStateCounter), 100);
//			 HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t DO hallPosition: %u\r\n", HALL_M1.HallState), 100);
			 //HAL_Delay(1000);
//while(HALL_M1.HallStateCounter != HALL_M1.HallCounterHomePosition){
	if( HALL_M1.HallStateCounter != HALL_M1.HallCounterHomePosition && moveMotor) //Default Position 0
	  	  {
			 HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t MOVING COunter: %u\r\n", HALL_M1.HallStateCounter), 100);
			 HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\t MOVING hallposition: %u\r\n", HALL_M1.HallState), 100);
			// HAL_Delay(1000);
	  		  if(stepA && pendingPhase)
	  		  {
	  //			  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "in phase control\n"), 100);
	  			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  			  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
	  			    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	  			  stepA =0 ;
	  			  stepAB=1;
	  			  pendingPhase = 0;
	  			  execute++;
	  			  i++;
	  		  }
	  		  if(stepAB && pendingPhase)
	  		  {
	  //			  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "in phase control\n"), 100);
	  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
	  		  	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
	  		  	   stepAB =0 ;
	  		  	   stepB=1;
	  		  	   pendingPhase = 0;
	  		  	}
	  		  if(stepB && pendingPhase)
	  		  	{
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
				  stepB = 0;
				  stepBC =1;
				  pendingPhase = 0;
	  		  	}
	  		  if(stepBC && pendingPhase)
	  		  	{
	  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pulse);
	  		  	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
	  		  	    stepBC = 0;
	  		  	    stepC =1;
	  		  	    pendingPhase = 0;
	  		  	 }
	  	      if(stepC && pendingPhase)
	  	      	 {
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					stepC = 0;
					stepCA=1;
					pendingPhase = 0;
	  	         }
	  	      if(stepCA && pendingPhase)
	  	      	 {
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulse);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
					 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,pulse);
					stepC = 0;
					stepA=1;
					pendingPhase = 0;
	  	      	  }

	  	        pendingPhase = 1;

	  	  }
	  	  else
	  	  {
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
				  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
				  moveMotor = 0;
				  if(count == 0 )
				  {
						   oldhallstate =(uint16_t *) HALL_M1.HallState;
						   count =1;
				  }
					  execute=0;
					  HAL_Delay(0.1);

	  	  }

	  	 // presenthallstate = (uint16_t *) HALL_M1.HallState;

		  if( HALL_M1.HallState == 71 && (HALL_M1.HallStateHomePosition != HALL_M1.HallState))
					  {
						  if( HALL_M1.HallStateHomePosition < HALL_M1.HallState)
						  {
							  HALL_M1.HallStateCounter = HALL_M1.HallStateCounter-HALL_M1.HallState+1;
							 // execute = execute - oldhallstate;
						  }
						  else
						  {
							  HALL_M1.HallStateCounter = HALL_M1.HallStateCounter-HALL_M1.HallState-1;
							 // execute = execute - presenthallstate;
						  }

						 count =0;
						 execute=0;
					  }
//		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, sprintf(buffer, "\n at pole: %u\r\n", HALL_M1.HallStateCounter), 75);

		  execute = execute%7;
		 // HAL_Delay(10);
		  waitToMoveCounter++;
		  if(moveMotor == 0 && waitToMoveCounter >=50)
		  {
			  moveMotor =1;
			  waitToMoveCounter=0;
		  }
}
}//while(HALL_M1.HallStateCounter != 0);
//}
/*-- My code ends --*/
