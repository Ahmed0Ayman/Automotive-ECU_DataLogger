/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include "queue.h"
#include "stdio.h"
#include <string.h>
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

/* USER CODE BEGIN PV */

char CANMessID[] = "Mess ID Is : ";
char CAN_RTR[] = "Mess IS Remote Frame : ";
char CAN_DLC[] = " Mess Lenght : " ;




void StartDefaultTask(void *argument);
void ReadFile_Fun(void *argument);
void ParseCommand_Fun(void *argument);
void StoreMessage_Fun(void *argument);



static CANMesg_t CanMessReceived ;


/* USER CODE END Variables */
/* Definitions for defaultTask */

/* Definitions for ReadFileTask */
osThreadId_t ReadFileTaskHandle;
const osThreadAttr_t ReadFileTask_attributes = {
  .name = "ReadFileTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for ParseCommandTas */
osThreadId_t ParseCommandTasHandle;
const osThreadAttr_t ParseCommandTas_attributes = {
  .name = "ParseCommandTas",
.stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
///* Definitions for StoreMessageTas */
osThreadId_t StoreMessageTasHandle;
const osThreadAttr_t StoreMessageTas_attributes = {
  .name = "StoreMessageTas",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
///* Definitions for QueueCANReceive */
osMessageQueueId_t QueueCANReceiveHandle;
const osMessageQueueAttr_t QueueCANReceive_attributes = {
 .name = "QueueCANReceive"
};
//
osMessageQueueId_t QueueCommandHandle;
const osMessageQueueAttr_t QueueCommand_attributes = {
  .name = "QueueCommand"
};
//
///* Definitions for SemWaitCommand */
osSemaphoreId_t SemWaitCommandHandle;
const osSemaphoreAttr_t SemWaitCommand_attributes = {
  .name = "SemWaitCommand"
};
//
//
QueueHandle_t CANMessQueue ;
void transmit_uart(char * string){
	uint8_t len = strlen(string);
	HAL_UART_Transmit(&huart1, (uint8_t *)string, len, 200);
}

/* SDcard manage variables */
FATFS 	FATFS_Struct;
FIL 	File_Struct;
char 	FileBuffer[100];
FATFS *pfs;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;


/* CAN manage variables */
CAN_RxHeaderTypeDef Rxmessage ;
CAN_TxHeaderTypeDef TxMessage ;
CAN_FilterTypeDef FilterInit ;
uint32_t fifo0 = CAN_RX_FIFO0;
uint8_t Command = 0 ;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* this function hold all pre initialization step */
void FirstInitializationProcess(void)
{
	  TxMessage.DLC = 8;
	  TxMessage.ExtId =0;
	  TxMessage.IDE = CAN_ID_STD;
	  TxMessage.RTR =0;
	  TxMessage.StdId = 0x123;
	  RetargetInit(&huart1);
	  HAL_Delay(500);

	  	if( f_mount(&FATFS_Struct, "", 0) == FR_OK) {
	  		printf("Micro SD card is mounted successfully!\n");
	  	} else {
	  		printf("Micro SD card's mount error!\n");
	  	}

		if ( f_getfree("", &fre_clust, &pfs)== FR_OK) {
	  	totalSpace = (uint32_t) ((pfs->n_fatent - 2) * pfs->csize * 0.5);
	  	freeSpace = (uint32_t) (fre_clust * pfs->csize * 0.5);
	  	char mSz[12];
	  	sprintf(mSz, "%lu", freeSpace);
	  	printf("The free space is: %s \n",mSz);
	  	} else {
	  		printf("The free space could not be determined!\n");
	  	}

	  	/* Open file to read */

	  	printf("The SDcard is successfully mounted  \n");


	  	if ( f_open(&File_Struct, "file.txt", FA_OPEN_APPEND | FA_WRITE | FA_READ)== FR_OK) {
	  		printf("File opened for reading and checking the free space.\n");
	  	} else  {
	  		printf("File was not opened for reading and checking the free space!\n");
	  	}



	printf("their is only two available commands \n press 1 : to read all logged data \n press 2 : to flush file \n");



	  FilterInit.FilterActivation = CAN_FILTER_ENABLE ;
	  FilterInit.FilterBank = 2;
	  FilterInit.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	  FilterInit.FilterIdHigh = 0x123<<5;
	  FilterInit.FilterIdLow = 0;
	  FilterInit.FilterMaskIdHigh = 0x00;

	  FilterInit.FilterMode = CAN_FILTERMODE_IDMASK;
	  FilterInit.FilterScale = CAN_FILTERSCALE_32BIT;
	  HAL_CAN_ConfigFilter(&hcan1,&FilterInit);

	  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
	  HAL_UART_Receive_IT(&huart1, &Command, 1);

	  __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	  HAL_CAN_Start(&hcan1);


}

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
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  FirstInitializationProcess();


  SemWaitCommandHandle = osSemaphoreNew(1, 1, &SemWaitCommand_attributes);


  QueueCANReceiveHandle = osMessageQueueNew (8, sizeof(CAN_RxHeaderTypeDef), &QueueCANReceive_attributes);

  /* creation of ReadFileTask */
  ReadFileTaskHandle = osThreadNew(ReadFile_Fun, NULL, &ReadFileTask_attributes);

  /* creation of ParseCommandTas */
  ParseCommandTasHandle = osThreadNew(ParseCommand_Fun, NULL, &ParseCommandTas_attributes);

  /* creation of StoreMessageTas */
  StoreMessageTasHandle = osThreadNew(StoreMessage_Fun, NULL, &StoreMessageTas_attributes);

  QueueCommandHandle = osMessageQueueNew(2,sizeof( uint8_t), &QueueCommand_attributes);


  CANMessQueue= xQueueCreate( 7, sizeof(CANMesg_t));


  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//
void ReadFile_Fun(void *argument)
{
//  /* Infinite loop */
	uint8_t C =0 ;
for(;;)
{

	  	osMessageQueueGet(QueueCommandHandle, &C, 0,osWaitForever);



	switch(C)
	{

	case '1' :
			f_close(&File_Struct); /* close file after each write to be safe  */
			if(f_open(&File_Struct, "DataLogger.txt",FA_READ) == FR_OK) /* check if there is an insufficient spaces */
					printf("Data logger file is opened as read file successfully   \n");
				else
					printf("error in Data logger file  !!!!!!!!!!  \n");
			while (f_gets(FileBuffer, sizeof(FileBuffer), &File_Struct)) {
			printf("%s " ,FileBuffer);
		}
		break ;


		case '2' :
			printf("closing the file \n");
			f_close(&File_Struct); /* close file after each write to be safe  */

		break ;

		default :

		break ;
		}

  }
}


void ParseCommand_Fun(void *argument)
{
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }

}


void StoreMessage_Fun(void *argument)
{
	/* char CANMessID[] = "Mess ID Is : ";
char CAN_RTR[] = "Mess IS Remote Frame : ";
char CAN_DLC[] = " Mess Lenght " ;

	 *
	 * */

  for(;;)
 {


	memset(FileBuffer ,0 , sizeof(	FileBuffer));
	xQueueReceive(CANMessQueue,&CanMessReceived ,-1);
	if(CanMessReceived.CANHeader.IDE == CAN_ID_STD)
	sprintf(FileBuffer,"%s  %x \n",CANMessID ,CanMessReceived.CANHeader.StdId );
	else
		sprintf(FileBuffer,"%s  %x \n",CANMessID ,CanMessReceived.CANHeader.ExtId );

		f_puts(FileBuffer, &File_Struct);
		memset(FileBuffer ,0 , sizeof(	FileBuffer));

		if(CanMessReceived.CANHeader.RTR == CAN_RTR_DATA)
		{
			sprintf(FileBuffer,"%s %s\n",CAN_RTR ,"false" );
			f_puts(FileBuffer, &File_Struct);
			memset(FileBuffer ,0 , sizeof(	FileBuffer));
		sprintf(FileBuffer,"%s %x %x %x %x %x %x %x %x \n","CAN DATA :",CanMessReceived.MesgData[0], CanMessReceived.MesgData[1], \
																			CanMessReceived.MesgData[2], CanMessReceived.MesgData[3],  \
																			CanMessReceived.MesgData[4], CanMessReceived.MesgData[5],  \
																	        CanMessReceived.MesgData[6], CanMessReceived.MesgData[7]);
			f_puts(FileBuffer, &File_Struct);
			printf("%s",FileBuffer);

		}else{
			sprintf(FileBuffer,"%s %s\n",CAN_RTR ,"true ,no data " );
			f_puts(FileBuffer, &File_Struct);

		}

		sprintf(FileBuffer,"\n ***************************************************************\n");

 }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance) /* check uart instance first */
	{
		osMessageQueuePut(QueueCommandHandle, &Command, 0 ,0);

	}



}


/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
