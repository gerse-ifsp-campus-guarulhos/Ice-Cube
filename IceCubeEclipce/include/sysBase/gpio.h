/*
 * gpio.h
 *
 *  Created on: 6 de set de 2018
 *      Author: pibs
 */

#ifndef GPIO_H_
#define GPIO_H_



#include <stdint.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_gpio.h>

//=========================== A ============================//
#define GPIOA0		10
#define GPIOA1		11
#define GPIOA2		12
#define GPIOA3		13
#define GPIOA4		14
#define GPIOA5		15
#define GPIOA6		16
#define GPIOA7		17
#define GPIOA8		29
#define GPIOA9		30
#define GPIOA10		31
#define GPIOA11		32
#define GPIOA12		33
#define GPIOA13		34
#define GPIOA14		37
#define GPIOA15		38
//=========================== B ============================//
#define GPIOB0		18
#define GPIOB1		19
#define GPIOB2		20
#define GPIOB10		21
#define GPIOB11		22
#define GPIOB12		25
#define GPIOB13		26
#define GPIOB14		27
#define GPIOB15		28
#define GPIOB3		39
#define GPIOB4		40
#define GPIOB5		41
#define GPIOB6		42
#define GPIOB7		43
#define GPIOB8		45
#define GPIOB9		46
//=========================== C ============================//
#define GPIOC13		2
#define GPIOC14		3
#define GPIOC15		4
//=========================== D ============================//
#define GPIOD0		5
#define GPIOD1		6


void gpio_initAll	(bool dbg				);					// ENABLE bus GPIO
void gpio_Mode		(uint8_t pin, GPIOMode_TypeDef mode	);					// SET MODE PIN OPERATION
bool gpio_Read		(uint8_t pin				);					// read specific pin
void gpio_Write		(uint8_t pin, bool value		);					// write specific pin
void gpio_InitInterrupt	(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource,  EXTITrigger_TypeDef mode);

#endif /* GPIO_H_ */
