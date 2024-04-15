/*
 * dht11.c
 *
 *  Created on: Nov 10, 2023
 *      Author: kamil
 */


#include "dht11.h"

void microDelay (DHT11_t *dht11, uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(dht11->tim, 0);
  while (__HAL_TIM_GET_COUNTER(dht11->tim) < delay);
}

uint8_t DHT11_Start (DHT11_t *dht11)
{
	uint32_t pMillis, cMillis;
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = dht11->pin;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(dht11->gpio, &GPIO_InitStructPrivate); // set the pin as output
  HAL_GPIO_WritePin (dht11->gpio, dht11->pin, 0);   // pull the pin low
  HAL_Delay(20);   // wait for 20ms
  HAL_GPIO_WritePin (dht11->gpio, dht11->pin, 1);   // pull the pin high
  microDelay (dht11, 30);   // wait for 30us
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(dht11->gpio, &GPIO_InitStructPrivate); // set the pin as input
  microDelay (dht11, 40);
  if (!(HAL_GPIO_ReadPin (dht11->gpio, dht11->pin)))
  {
    microDelay (dht11, 80);
    if ((HAL_GPIO_ReadPin (dht11->gpio, dht11->pin))) Response = 1;
  }
  pMillis = HAL_GetTick();
  cMillis = HAL_GetTick();
  while ((HAL_GPIO_ReadPin (dht11->gpio, dht11->pin)) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}

uint8_t DHT11_Read (DHT11_t *dht11)
{
uint32_t pMillis, cMillis;
  uint8_t a,b;
  for (a=0;a<8;a++)
  {
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin (dht11->gpio, dht11->pin)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go high
      cMillis = HAL_GetTick();
    }
    microDelay (dht11,40);   // wait for 40 us
    if (!(HAL_GPIO_ReadPin (dht11->gpio, dht11->pin)))   // if the pin is low
      b&= ~(1<<(7-a));
    else
      b|= (1<<(7-a));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin (dht11->gpio, dht11->pin)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go low
      cMillis = HAL_GetTick();
    }
  }
  return b;}

void DHT11_Run(DHT11_t *dht11, uint8_t *array_data, int index)
{


	 if(DHT11_Start(dht11))
		      {

		 	 	array_data[index+2] = DHT11_Read(dht11); // Relative humidity integral
		 	 	DHT11_Read(dht11);
		        array_data[index] = DHT11_Read(dht11); // Celsius integral
		        array_data[index+1] = DHT11_Read(dht11); // Celsius decimal
		        DHT11_Read(dht11); // Check sum
		      }
}
void DHT11_init(DHT11_t *dht11, TIM_HandleTypeDef *tim, GPIO_TypeDef *gpio, uint16_t pin)
{
	dht11->tim = tim;
	dht11->gpio = gpio;
	dht11->pin = pin;
	HAL_TIM_Base_Init(dht11->tim);
	HAL_TIM_Base_Start(dht11->tim);
}


