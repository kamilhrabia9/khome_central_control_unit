/*
 * dht11.h
 *
 *  Created on: Nov 10, 2023
 *      Author: kamil
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_


#include "tim.h"
#include "gpio.h"
#include <stdbool.h>




typedef struct
{
 TIM_HandleTypeDef   *tim;
 GPIO_TypeDef        *gpio;
 uint16_t            pin;
}DHT11_t;


void microDelay (DHT11_t *dht11, uint16_t delay);
uint8_t DHT11_Start (DHT11_t *dht11);
uint8_t DHT11_Read (DHT11_t *dht11);
void DHT11_Run(DHT11_t *dht11, uint8_t *array_data, int index);
void DHT11_init(DHT11_t *dht11, TIM_HandleTypeDef *tim, GPIO_TypeDef *gpio, uint16_t pin);



#endif /* INC_DHT11_H_ */
