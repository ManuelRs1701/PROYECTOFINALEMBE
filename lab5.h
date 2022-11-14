#ifndef LAB5_H_
#define LAB5_H_

#include <stdint.h>                            					// Standard Integer - data type definitions
#include <stdio.h>                             					// Standard I/O definitions
#include <stdbool.h>																		// Standard Boolean definitions
#include <stdarg.h>
#include <math.h>

#include "stm32f3xx.h"																 // Platform specific header (HW definition)
#include "stm32f3xx_hal.h" 														 // HAL Drivers



void exercise_1(void);

void setup_gpio(void);
void setup_timer(void);
void setup_adc(void);
void ADC1_2_IRQHandler(void);

void TIM3_IRQHandler(void);
void delay_us(int);
void setup_delay(void);




#endif