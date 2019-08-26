/*
 * adc.h
 *
 *  Created on: 12 de set de 2018
 *      Author: pibs
 */




#ifndef ADC_H_
#define ADC_H_

#include <stm32f10x.h>
#include <../include/sysBase/gpio.h>

// pino fisico para ADCs
#define ADC0_PIN	GPIOA0
#define ADC1_PIN	GPIOA1
#define ADC2_PIN	GPIOA2
#define ADC3_PIN	GPIOA3
#define ADC4_PIN	GPIOA4
#define ADC5_PIN	GPIOA5
#define ADC6_PIN	GPIOA6
#define ADC7_PIN	GPIOA7
#define ADC8_PIN	GPIOB0
#define ADC9_PIN	GPIOB1

// canal do ADC
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7
#define ADC_CH8		8
#define ADC_CH9		9



void adc1_Init(void);
u16 adc1_Read(u8 channel);


#endif /* ADC_H_ */
