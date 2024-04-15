/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht.h"
#include "dht11.h"
#include "string.h"
#include "OneWire.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENSOR_DATA_SIZE 17
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
GPIO_TypeDef* bool_switches[12] = {OUT_G_GATE_GPIO_Port, OUT_LAMP_GPIO_Port, OUT_LAMP_PATH_GPIO_Port, WATERING_GPIO_Port, LIV_LAMP_1_GPIO_Port, LIV_LAMP_2_GPIO_Port, BED_LAMP_1_GPIO_Port, BED_LAMP_2_GPIO_Port, GAR_LAMP_GPIO_Port, PUMP_WATER_GPIO_Port, LIV_HEAT_GPIO_Port, BED_HEAT_GPIO_Port};
uint16_t bool_switches_pin[12] = {OUT_G_GATE_Pin, OUT_LAMP_Pin, OUT_LAMP_PATH_Pin, WATERING_Pin, LIV_LAMP_1_Pin, LIV_LAMP_2_Pin, BED_LAMP_1_Pin, BED_LAMP_2_Pin, GAR_LAMP_Pin, PUMP_WATER_Pin, LIV_HEAT_Pin, BED_HEAT_Pin};
GPIO_TypeDef* temp_switches[4] = {LIV_HEAT_GPIO_Port,BED_HEAT_GPIO_Port,HEATER_WATER_GPIO_Port,HEATER_CH_GPIO_Port};//0- liV_heat, 1-bed_heat, 2-CWU_heater, 3- cwo heater
uint16_t temp_switches_pin[4]= {LIV_HEAT_Pin,BED_HEAT_Pin,HEATER_WATER_Pin,HEATER_CH_Pin};//0- liV_heat, 1-bed_heat, 2-CWU_heater, 3- cwo heater
GPIO_TypeDef* gate_io_port[4] = {DRW_GATE_GPIO_Port,G_GATE_GPIO_Port,LIV_WIN_GPIO_Port,BED_WIN_GPIO_Port};
uint16_t gate_io_pin[4] = {DRW_GATE_Pin,G_GATE_Pin,LIV_WIN_Pin,BED_WIN_Pin};
GPIO_TypeDef* blind_ac_io_port[8] = {LIV_BLIND_1_GPIO_Port,LIV_FAN_1_GPIO_Port,BED_BLIND_1_GPIO_Port,BED_FAN_1_GPIO_Port,LIV_BLIND_2_GPIO_Port,LIV_FAN_2_GPIO_Port,BED_BLIND_2_GPIO_Port,BED_FAN_2_GPIO_Port};
uint16_t blind_ac_io_pin[8] = {LIV_BLIND_1_Pin,LIV_FAN_1_Pin,BED_BLIND_1_Pin,BED_FAN_1_Pin,LIV_BLIND_2_Pin,LIV_FAN_2_Pin,BED_BLIND_2_Pin,BED_FAN_2_Pin};

uint8_t uart_index=0;

uint8_t set_temp[10]; //0- LIV_Sensor, 1- BED_Sensor, 2 - CWO_Sensor, 3- LIV_UART, 4- BED_UART, 5-CWO_uart rest is bool for heating, 9-cwu sensor

extern float Temp[MAXDEVICES_ON_THE_BUS];
DHT11_t dht11_1;
DHT11_t dht11_2;
uint8_t sensor_data[SENSOR_DATA_SIZE];
uint8_t dummydata = 0;
uint8_t rxdata[3];
uint8_t txdata[3];
uint8_t switch_off[6] = {0,0,0,0,0,0}; //DRW, GG, LW, BW, PIR LIV, PIR BED
uint8_t weather_state = 0;

DHT_t     am2301;
bool      am2301_ok;
float t1,h1;

uint8_t mode = 0; // 0 - manual, 1 - auto
uint8_t heatingStart = 0; // 0 - off 1 - on
uint8_t cw_temp = 0;
uint8_t gate_state[4]={0,0,0,0};
uint8_t uart_call = 0;

uint8_t t_timer_index = 0;
uint8_t temp_on = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void tempUartTx(int txIndex, int tempIndex, uint8_t *data, uint8_t *tx)
{
	tx[0] = txIndex;
	tx[1] = data[tempIndex];
	tx[2] = data[tempIndex+1];
	HAL_UART_Transmit (&huart3, tx, 3, 10);
};

void humUartTx(int txIndex, int tempIndex, uint8_t *data, uint8_t *tx)
{
	tx[0] = txIndex;
	tx[1] = data[tempIndex];
	tx[2] = 0;
	HAL_UART_Transmit (&huart3, tx, 3, 10);
};

void am2301_Run(void)
{
    am2301_ok = DHT_readData(&am2301,&t1,&h1,sensor_data,3);
    if(am2301_ok)
    {

    }
}

void temp_sensors_measure(uint8_t index, uint8_t *t_on)
{

	if(*t_on)
	{
		switch (index)
		{
		case 0:
			DHT11_Run(&dht11_1,sensor_data,0); // read from liv
			set_temp[0] =sensor_data[0];
			tempUartTx(0, 0, sensor_data, txdata);
			break;
		case 1:
			humUartTx(7, 2, sensor_data, txdata); // send uart hum from liv
			break;
		case 2:
			DHT11_Run(&dht11_2,sensor_data,6); //temp from bed
			set_temp[1] = sensor_data[6];
			tempUartTx(1, 6, sensor_data, txdata);
			break;
		case 3:
			humUartTx(8, 5, sensor_data, txdata);// send hum from bed
			break;
		case 4:
			am2301_Run(); //temp from out
			tempUartTx(2, 3, sensor_data, txdata);
			break;
		case 5:
			humUartTx(9, 8, sensor_data, txdata); // hum from out
			break;
		case 6:
			get_Temperature(sensor_data,9); // get temp from ds18

			tempUartTx(3, 9, sensor_data, txdata);
			break;
		case 7:
			tempUartTx(4, 11, sensor_data, txdata);
			break;
		case 8:
			tempUartTx(5, 13, sensor_data, txdata);
			set_temp[9] = sensor_data[13];
			break;
		case 9:
			tempUartTx(6, 15, sensor_data, txdata);
			set_temp[2] = sensor_data[15];
			break;
		default:
			break;
		}

		*t_on = 0;
	}

}


void central_water_check(uint8_t *temp_data, uint8_t temp_on, uint8_t *selector,uint8_t *heatingStart)
{
	if(!*selector)
	{
		if (temp_data[9]<temp_on)
		{
		*heatingStart = 1;
		*selector = 1;
		}
	}
	else
	{
		if (temp_data[9]>temp_on)
		{
		*heatingStart = 1;
		*selector = 0;
		}
	}

}

void temperature_regulation(uint8_t *temp_data, GPIO_TypeDef* *gpio_port, uint16_t *gpio_pin, uint8_t *heatStart,uint8_t *cw_temp)
{

central_water_check(temp_data, 40, cw_temp, heatStart);

if(*heatStart)
{
if (temp_data[6]||temp_data[7])
{
 if(temp_data[9]<40)
	{
	HAL_GPIO_WritePin(gpio_port[3],gpio_pin[3],GPIO_PIN_SET);

	}
else
	{
	HAL_GPIO_WritePin(gpio_port[3],gpio_pin[3],GPIO_PIN_RESET);
	}

}
for(int i=0; i<3;i++)
	{
if(temp_data[6+i])
	{
		if(temp_data[i]<temp_data[3+i])
		{

			HAL_GPIO_WritePin(gpio_port[i],gpio_pin[i],GPIO_PIN_SET);
		}

		else
		{
			HAL_GPIO_WritePin(gpio_port[i],gpio_pin[i],GPIO_PIN_RESET);
		}
	}
else
{
			HAL_GPIO_WritePin(gpio_port[i],gpio_pin[i],GPIO_PIN_RESET);
}

}
 *heatStart = 0;

	}
};

void timer_switch_off(void)
{
	for(int i = 0;i<2;i++) // GG and DRW
				{
				if(switch_off[i]>4)
					{
					switch_off[i] =0;
					HAL_GPIO_WritePin(gate_io_port[i],gate_io_pin[i],GPIO_PIN_RESET);
					}

				}
	for(int i =2;i<4;i++)// windows off
	{
					if(switch_off[i]>10)
						{
						switch_off[i] =0;
						HAL_GPIO_WritePin(gate_io_port[i],gate_io_pin[i],GPIO_PIN_RESET);
						}


					}
	if(switch_off[4]> 3)
	{
		switch_off[4]=0;
		HAL_GPIO_WritePin(LIV_LAMP_1_GPIO_Port, LIV_LAMP_1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LIV_LAMP_2_GPIO_Port, LIV_LAMP_2_Pin,GPIO_PIN_RESET);
	}
	if(switch_off[5]> 3)
		{
			switch_off[5]=0;
			HAL_GPIO_WritePin(BED_LAMP_1_GPIO_Port, BED_LAMP_1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BED_LAMP_2_GPIO_Port, BED_LAMP_2_Pin,GPIO_PIN_RESET);
		}
}


void weather_check(void)
{
// 0 - sun, 1 - cloudy, 2 - rain, 3 - night, 4 - rainy night

if(t_timer_index == 10)
{
	uint16_t adc_sun = 0;
	uint16_t sunny = 3000;
	//uint16_t cloudy = 1000;
	uint16_t night = 1700;
	uint16_t rain =0;
	uint8_t state = 0;
	txdata[0]=16;
	txdata[2]=0;

HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 1);
adc_sun = HAL_ADC_GetValue(&hadc1);
if(HAL_GPIO_ReadPin(RAIN_GPIO_Port, RAIN_Pin)== GPIO_PIN_SET)rain=1;

if(adc_sun>=sunny && !rain )state = 0;
if(adc_sun<sunny && adc_sun>=night && !rain )state = 1;
if(adc_sun>=night && rain )state = 2;
if( adc_sun<night && !rain )state = 3;
if( adc_sun<night && rain )state = 4;

if(state != weather_state)
{
txdata[1] = state;
HAL_UART_Transmit (&huart3, txdata, 3, 10);
weather_state = state;
}

}



//HAL_UART_Transmit (&huart3, txdata, 3, 10);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if (htim->Instance == htim4.Instance)
		  {
			t_timer_index = t_timer_index +1;
			temp_on = 1;
			if (t_timer_index >11) //11- adc
			{
				t_timer_index =0;
			}

			for(int i = 0;i<6;i++) // liczenie switch off
			{
			if(switch_off[i])switch_off[i] =switch_off[i]+1;
			}
		  }
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	ds1[0] = 0;
	uart_call =9;
	 t_timer_index = 0;
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
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart3, rxdata, sizeof(rxdata));
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_Base_Start_IT(&htim4); // timer for temp index
  DHT_init(&am2301,DHT_Type_AM2301,&htim3,64,SENSOR_GPIO_Port,SENSOR_Pin);
  DHT11_init(&dht11_1, &htim1, DHT11_GPIO_Port, DHT11_Pin);
  DHT11_init(&dht11_2, &htim1, DHT12_GPIO_Port, DHT12_Pin);
  get_ROMid();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 temp_sensors_measure(t_timer_index, &temp_on);
	 temperature_regulation(set_temp, temp_switches, temp_switches_pin,&heatingStart,&cw_temp);
	 weather_check();
	 timer_switch_off();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */


//HAL_GPIO_WritePin(devices[rxdata[0]],devices_pin[rxdata[0]], rxdata[1]);
	uart_call = rxdata[0];

	if(rxdata[0]< 10)
	{
		HAL_GPIO_WritePin(bool_switches[rxdata[0]],bool_switches_pin[rxdata[0]], rxdata[1]);
	;
	}
	else if (rxdata[0]==10) // mode
	{
		mode = rxdata[1];
	}
	else if (rxdata[0]>10 && rxdata[0]< 14) // Heating Bools
	{
		heatingStart = 1;
		set_temp[rxdata[0]-11+6] =  rxdata[1];

	}
	else if(rxdata[0]>13 && rxdata[0]< 18) // uart do gateRun
	{
		if(rxdata[1] == 1 || rxdata[1] == 3)
		{
		HAL_GPIO_WritePin(gate_io_port[rxdata[0]-14],gate_io_pin[rxdata[0]-14],GPIO_PIN_SET);
		switch_off[rxdata[0]-14]=1 ;
		gate_state[rxdata[0]-14] = rxdata[1];
		}
		if(rxdata[0]>15)
		{
			txdata[0] =rxdata[1]-4;
			txdata[2] =0;
			if(rxdata[1] == 1)txdata[1] =2;
			if(rxdata[1] == 3)txdata[1] =0;
			HAL_UART_Transmit (&huart3, txdata, 3, 10);
		}
	}
	else if(rxdata[0]>17 && rxdata[0]< 22) //blind ac
	{
		if(rxdata[1] == 0)
		{
			HAL_GPIO_WritePin(blind_ac_io_port[rxdata[0]-18],blind_ac_io_pin[rxdata[0]-18],GPIO_PIN_RESET);
			HAL_GPIO_WritePin(blind_ac_io_port[rxdata[0]-18 + 4],blind_ac_io_pin[rxdata[0]-18 + 4],GPIO_PIN_RESET);
		}
		else if(rxdata[1] == 1)
		{
			HAL_GPIO_WritePin(blind_ac_io_port[rxdata[0]-18],blind_ac_io_pin[rxdata[0]-18],GPIO_PIN_SET);
			HAL_GPIO_WritePin(blind_ac_io_port[rxdata[0]-18 + 4],blind_ac_io_pin[rxdata[0]-18 + 4],GPIO_PIN_RESET);
		}
		else if(rxdata[1] == 2)
		{
			HAL_GPIO_WritePin(blind_ac_io_port[rxdata[0]-18],blind_ac_io_pin[rxdata[0]-18],GPIO_PIN_SET);
			HAL_GPIO_WritePin(blind_ac_io_port[rxdata[0]-18 + 4],blind_ac_io_pin[rxdata[0]-18 + 4],GPIO_PIN_SET);
		}
	}

	else if(rxdata[0]>21 && rxdata[0]< 25) // set temp from LIV and BED and cwo
		{
			set_temp[rxdata[0]-22+3] =  rxdata[1];
			heatingStart = 1;
		}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == am2301.pin)
	  {
	    DHT_pinChangeCallBack(&am2301);
	  }
	if(GPIO_Pin == DRW_GATE_OPEN_Pin)
	  {
		if(gate_state[0] == 1)
		{
		gate_state[0] = 2;
		txdata[0] = 10;
		txdata[1] = 2 ;
		txdata[2] = 0;
		HAL_UART_Transmit (&huart3, txdata, 3, 10);
		}
	  }
	if(GPIO_Pin == DRW_GATE_CLOSE_Pin)
	  {
		if(gate_state[0] == 3)
		{
		gate_state[0] = 0;
		txdata[0] = 10;
		txdata[1] = 0;
		txdata[2] = 0;
		HAL_UART_Transmit (&huart3, txdata, 3, 10);
		}
	  }
	if(GPIO_Pin == G_GATE_OPEN_Pin)
		  {
		if(gate_state[1] == 1)
		{
			gate_state[1] = 2;
			txdata[0] = 11;
			txdata[1] = 2 ;
			txdata[2] = 0;
			HAL_UART_Transmit (&huart3, txdata, 3, 10);
			uart_index = uart_index+1;
		}
		  }
		if(GPIO_Pin == G_GATE_CLOSE_Pin)
		  {
			if(gate_state[1] == 3)
			{
			gate_state[1] = 0;
			txdata[0] = 11;
			txdata[1] = 0 ;
			txdata[2] = 0;
			HAL_UART_Transmit (&huart3, txdata, 3, 10);
			uart_index = uart_index+1;
			}
		  }
		if(GPIO_Pin == LIV_WIN_STATUS_Pin)
		 {
			txdata[0] = 12;
			txdata[1] = 0 ;
			txdata[2] = 0;

			HAL_UART_Transmit (&huart3, txdata, 3, 10);
		 }
		if(GPIO_Pin == BED_WIN_STATUS_Pin)
		{
			txdata[0] = 13;
			txdata[1] = 0 ;
			txdata[2] = 0;
			HAL_UART_Transmit (&huart3, txdata, 3, 10);
		}
		if(GPIO_Pin == LIV_MOVE_Pin)
		{
			if(mode) //automatic zaswiecenie swiatel
			{
				switch_off[4]=1;
				HAL_GPIO_WritePin(LIV_LAMP_1_GPIO_Port, LIV_LAMP_1_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(LIV_LAMP_2_GPIO_Port, LIV_LAMP_2_Pin,GPIO_PIN_SET);
			}
		}
		if(GPIO_Pin == BED_MOVE_Pin)
			{
				if(mode)
				{
					switch_off[5]=1;
					HAL_GPIO_WritePin(BED_LAMP_1_GPIO_Port, BED_LAMP_1_Pin,GPIO_PIN_SET);
					HAL_GPIO_WritePin(BED_LAMP_2_GPIO_Port, BED_LAMP_2_Pin,GPIO_PIN_SET);
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
