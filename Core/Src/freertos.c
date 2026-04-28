/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "i2c.h"
#include "functions.h"
#include "task.h"
#include "sensor_message.h"
#include "generated_points.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USE_UART_PRINTF

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t xDefaultTaskHandle;
TaskHandle_t xLedTaskHandle;
TaskHandle_t xI2C_TaskHandle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId I2C_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartI2CTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of I2C_Task */
  osThreadDef(I2C_Task, StartI2CTask, osPriorityIdle, 0, 128);
  I2C_TaskHandle = osThreadCreate(osThread(I2C_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
//  xI2C_TaskHandle = (TaskHandle_t)I2C_TaskHandle;
  xI2C_TaskHandle = (TaskHandle_t)I2C_TaskHandle;



  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {

	    // Process I2C error recovery only (RX/TX are handled in I2C task via notifications)
	    if (i2c_err_flag)
	    {
	      i2c_err_flag = 0;
	      /*uart_printf("I2C: ERROR detected, code=0x%08lX - reinitializing I2C\r\n", i2c_last_error);*/

	      // Deinit and re-init the peripheral (do heavy work in main context)
	      if (HAL_I2C_DeInit(&hi2c1) != HAL_OK)
	      {
	        /*uart_printf("I2C: DeInit failed\r\n");*/
	      }
	      // Reconfigure/Init peripheral using the generated init routine
	      MX_I2C1_Init();

	      // Re-arm TX and RX
	      HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t *)buffer_tx, 3);
	      HAL_I2C_Slave_Receive_IT(&hi2c1, rxCmd, 2);

	      /*uart_printf("I2C: Re-armed TX and RX after error\r\n");*/
	    }
		osDelay(10);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartI2CTask */
/**
* @brief Function implementing the I2C_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2CTask */
void StartI2CTask(void const * argument)
{
  /* USER CODE BEGIN StartI2CTask */
  /* Infinite loop */
  for(;;)
  {


	  uint32_t ev =0; //changer de place plus tard dans le bon dossier

	  	  xTaskNotifyWait(0, 0xFFFFFFFFu, &ev, portMAX_DELAY); // wait indefinitely for notification



	  	  if((ev & I2C_EVENT_RX_COMPLETE) == I2C_EVENT_RX_COMPLETE)
	  	  {

	  		uint8_t cmd = rxCmd[0];
	  		switch(cmd)
	  		{
	  		    case COMMAND_PRE_ALARM_CONFIRMATION: // 0x2C
	  		        break;

	  		    case COMMAND_ALARM_CONFIRMATION_1: // 0x2E
	  		        break;

	  		    case COMMAND_ALARM_CONFIRMATION_2: // 0x40
	  		        break;

	  		    case COMMAND_ADDRESS_DISABLED: // 0x80
	  		        break;

	  		    case COMMAND_SWITCH_OFF_LIMIT_LINE: // 0x81
	  		        break;

	  		    case COMMAND_SWITCH_ON_LIMIT_LINE: // 0x82
	  		        break;

	  		    case COMMAND_RESET_ALARM_LINE: // 0x91
//	  		        // Reset all points to standby
//	  		        for (uint16_t i = 0; i < generated_points_count; i++)
//	  		        {
//	  		            generated_points[i].b2 = 0x22;  // STATE_STANDBY
//	  		        }
//	  		        current_tx_index = 0;
//	  		        selected_point_index = 0;
	  		        break;

	  		    case COMMAND_RESET_GROUP_ALARM: // 0x92
	  		        // Reset all points to standby
//	  		        for (uint16_t i = 0; i < generated_points_count; i++)
//	  		        {
//	  		            generated_points[i].b2 = 0x22;  // STATE_STANDBY
//	  		        }
//	  		        current_tx_index = 0;
//	  		        selected_point_index = 0;
	  		        break;

	  		    case COMMAND_RESET_SIGNALLING_UNIT: // 0x93
	  		        break;

	  		    case COMMAND_GROUP_RESET_LIMIT_CARD: // 0x94
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_1_NO_FLASH: // 0x97
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_2_NO_FLASH: // 0x98
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_3_NO_FLASH: // 0x99
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_4_NO_FLASH: // 0x9A
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_5_NO_FLASH: // 0x9B
	  		        break;

	  		    case COMMAND_READ_ADDRESS_INSPECTION_DATE: // 0x9C
	  		        break;

	  		    case COMMAND_SET_INSPECTION_DATE: // 0x9D
	  		        break;

	  		    case COMMAND_ACTIVATE_RELAY: // 0xA1
	  		        break;

	  		    case COMMAND_RESET_RELAY: // 0xA7
	  		        break;

	  		    case COMMAND_DETECTOR_TEST: // 0xB1
	  		        break;

	  		    case COMMAND_READ_MANUFACTURE_DATE: // 0xB4
	  		        break;

	  		    case COMMAND_READ_CONTAMINATION: // 0xB5
	  		        break;

	  		    case COMMAND_READ_SENSITIVITY: // 0xB6
	  		        break;

	  		    case COMMAND_READ_INSPECTION_DATE: // 0xB7
	  		        break;

	  		    case COMMAND_MULTI_READ_MANUFACTURE_DATE: // 0xB8
	  		        break;

	  		    case COMMAND_MULTI_READ_CONTAMINATION: // 0xB9
	  		        break;

	  		    case COMMAND_MULTI_READ_SENSITIVITY: // 0xBA
	  		        break;

	  		    case COMMAND_MULTI_READ_INSPECTION_DATE: // 0xBB
	  		        break;

	  		    case COMMAND_MULTI_READ_END: // 0xBC
	  		        break;

	  		    case COMMAND_IMPORT_DETECTOR: // 0xC0
	  		        step = step_init;
	  		        current_tx_index = 0;
	  		        buffer_tx = get_generated_point_to_send(current_tx_index);
	  		        HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t *)buffer_tx, 3);
//	  		        HAL_GPIO_WritePin(LED_INIT_GPIO_Port, LED_INIT_Pin, GPIO_PIN_SET); // indicate import started
	  		        break;

	  		    case COMMAND_EXPLOSION_LIMIT_LINE_OFF: // 0xD0
	  		        break;

	  		    case COMMAND_EXPLOSION_LIMIT_LINE_ON: // 0xD1
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_1_FLASH: // 0xD7
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_2_FLASH: // 0xD8
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_3_FLASH: // 0xD9
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_4_FLASH: // 0xDA
	  		        break;

	  		    case COMMAND_SENSITIVITY_LEVEL_5_FLASH: // 0xDB
	  		        break;

	  		    case COMMAND_ALARM_OUTPUT_OFF: // 0xE0
	  		        break;

	  		    case COMMAND_ALARM_OUTPUT_ON: // 0xE1
	  		        break;

	  		    case COMMAND_ICHECK: // 0xFE
	  		        break;

	  		    default:
	  		        break;
	  		}

	  	  }

	  	if ((ev & I2C_EVENT_TX_COMPLETE) == I2C_EVENT_TX_COMPLETE)
	  	{
	  	    // Check if a special message is pending
	  	    if (special_message_pending)
	  	    {
	  	        // Send the special message (already copied to tx_0)
	  	        buffer_tx = &tx_0;
	  	        special_message_pending = 0; // Clear flag after sending once
	  	    }
	  	    else
	  	    {
	  	        // Continue with normal generated_points cycle
	  	        current_tx_index++;
	  	        if (current_tx_index >= generated_points_count)
	  	        {
	  	            current_tx_index = 1; // restart from first real point, not 0x00 0x00 0x00
	  	        }
	  	        buffer_tx = get_generated_point_to_send(current_tx_index);
	  	    }

	  	    if (step == step_init)
	  	    {
	  	    	HAL_GPIO_WritePin(LED_INIT_GPIO_Port, LED_INIT_Pin, GPIO_PIN_RESET); // indicate import completed
	  	    	HAL_GPIO_WritePin(LED_LOOP_GPIO_Port, LED_LOOP_Pin, GPIO_PIN_SET); // indicate loop mode started
	  	        osDelay(1000); // brief delay for visual feedback
	  	        step = step_loop;
	  	    }

	  	    //allumer la LED init a chaque fin de trasmission FE FE FE pour indiquer que la boucle est terminée et que le buffer_tx est de nouveau prêt à être utilisé pour un message spécial
	  	    if (buffer_tx == &buffer_tx_end)
	  	    {
	  	        HAL_GPIO_WritePin(LED_INIT_GPIO_Port, LED_INIT_Pin, GPIO_PIN_SET); // indicate loop completed and ready for special message
	  	    }
	  	    HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t *)buffer_tx, 3);

	  	    HAL_GPIO_TogglePin(LED_TX_COMPLETE_GPIO_Port, LED_TX_COMPLETE_Pin); // Toggle an LED to indicate TX complete





	  	}


	  	if ((ev & EVENT_BTN_SPECIAL_SEL) == EVENT_BTN_SPECIAL_SEL)
	  		  	{
	  		  	    // Cycle through available points
	  		  		selected_special_message_index++;
	  		  	    if (selected_special_message_index >= SPECIAL_MESSAGE_MAX)
	  		  	    {
	  		  	   	 selected_special_message_index = SPECIAL_MESSAGE_NONE; // wrap around to "none" state
	  		  	    }

	  		  	    // Visual feedback: LED indicates selection mode
	  		  	    HAL_GPIO_WritePin(LED_SPE_SELECT_GPIO_Port, LED_SPE_SELECT_Pin, GPIO_PIN_SET);
	  		  	    HAL_GPIO_WritePin(LED_SPE_VALIDATE_GPIO_Port, LED_SPE_VALIDATE_Pin, GPIO_PIN_RESET);

	  		  	}

	  	if (ev & EVENT_BTN_SPECIAL_VAL)
	  	{
	  		if (selected_special_message_index > SPECIAL_MESSAGE_NONE &&
	  	        selected_special_message_index < SPECIAL_MESSAGE_MAX)
	  	    {
	  			int index = selected_special_message_index - 1; // adjust index to account for SPECIAL_MESSAGE_NONE at 0
	  			tx_0.address = special_messages[index].address;
	  			tx_0.type = special_messages[index].type;
	  			tx_0.status = special_messages[index].status;

	  			// Set flag to send special message on I2C_EVENT_TX_COMPLETE
	  			special_message_pending = 1;

	  	        HAL_GPIO_WritePin(LED_SPE_VALIDATE_GPIO_Port, LED_SPE_VALIDATE_Pin, GPIO_PIN_SET); // indicate special message selected
	  	        HAL_GPIO_WritePin(LED_SPE_SELECT_GPIO_Port, LED_SPE_SELECT_Pin, GPIO_PIN_RESET); // exit selection mode

	  	    }

	  	    selected_special_message_index = SPECIAL_MESSAGE_NONE; // reset selection after validation

#ifdef USE_UART_PRINTF
	  	    uart_printf("Special message validated: index %d | [%02X %02X %02X]\r\n",
	  	        selected_special_message_index,
	  	        tx_0.address,
	  	        tx_0.type,
	  	        tx_0.status
	  	    );
	  	    #endif


	  	}

	  	if ((ev & EVENT_BTN_NEXT) == EVENT_BTN_NEXT)
	  	{
	  	    if (selected_point_index >= GENERATED_POINTS_MAX - 1)
	  	    {
	  	        selected_point_index = GENERATED_POINT_NONE + 1;
	  	    }
	  	    else
	  	    {
		  	    selected_point_index++;
	  	    }
//  	        HAL_GPIO_WritePin(LED_CHANGE_POINT_STATUS_GPIO_Port, LED_CHANGE_POINT_STATUS_Pin, GPIO_PIN_SET);

	  	    update_selected_point_leds();

#ifdef USE_UART_PRINTF
	  	    uart_printf("current point index: %d\r\n", selected_point_index);
	  	   #endif

	  	}
	  	if ((ev & EVENT_BTN_PREV) == EVENT_BTN_PREV)
	  	{
	  	    if (selected_point_index <= GENERATED_POINT_NONE + 1)
	  	    {
	  	        selected_point_index = GENERATED_POINTS_MAX - 1;
	  	    }
	  	    else
	  	    {
	  	        selected_point_index--;
	  	    }

	  	    HAL_GPIO_WritePin(LED_CHANGE_POINT_STATUS_GPIO_Port, LED_CHANGE_POINT_STATUS_Pin, GPIO_PIN_SET);
	  	    update_selected_point_leds();

#ifdef USE_UART_PRINTF
	  	    uart_printf("current point index: %d\r\n", selected_point_index);
	  	    #endif

	  	}
	  	if ((ev & EVENT_BTN_CHANGE_STATUS) == EVENT_BTN_CHANGE_STATUS)
	  	{
	  	    if ((selected_point_index > GENERATED_POINT_NONE) &&
	  	        (selected_point_index < GENERATED_POINTS_MAX))
	  	    {
	  	        if (generated_points[selected_point_index].status == STATE_STANDBY)
	  	        {
	  	            generated_points[selected_point_index].status = STATE_FAULT;
	  	        }
	  	        else
	  	        {
	  	            generated_points[selected_point_index].status = STATE_STANDBY;
	  	        }

#ifdef USE_UART_PRINTF
uart_printf("curent point index: %d, new status: 0x%02X\r\n", selected_point_index, generated_points[selected_point_index].status);
#endif
	  	        HAL_GPIO_WritePin(LED_CHANGE_POINT_STATUS_GPIO_Port, LED_CHANGE_POINT_STATUS_Pin, GPIO_PIN_SET);
	  	        update_selected_point_leds();
	  	    }

	  	}

	  	if ((ev & EVENT_BTN_RESET_ALL_STATUS) == EVENT_BTN_RESET_ALL_STATUS)
	  	{
	  		for (uint8_t i = 1; i < generated_points_count - 1; i++)
	  		{
	  		    if (generated_points[i].status != STATE_STANDBY)
	  		    {
	  		        generated_points[i].status = STATE_STANDBY;
	  		    }
	  		}
	  		// Mettre à jour l'affichage des LEDs pour le point sélectionné
	  		update_selected_point_leds();
	  	}


    osDelay(1);
  }
  /* USER CODE END StartI2CTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

