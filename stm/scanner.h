#ifndef SCANNER_H_H
#define SCANNER_H_

/* STM32 includes */
#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_adc.h>

#include "common.h"

#define MOTOR_PORT 	GPIOC
#define MOTOR_PIN_0  	GPIO_Pin_0
#define MOTOR_PIN_1  	GPIO_Pin_1
#define MOTOR_PIN_2  	GPIO_Pin_2
#define MOTOR_PIN_3  	GPIO_Pin_3

#define ALL_MOTOR_PINS (MOTOR_PIN_0 | MOTOR_PIN_1 | MOTOR_PIN_2 | MOTOR_PIN_3)

void scan_hw_init();
void step_motor(int steps);
int get_distance();
void start_measure();
void scan();

#endif
