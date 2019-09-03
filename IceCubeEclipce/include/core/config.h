/*
 * config.h
 *
 *  Created on: 14 de ago de 2019
 *      Author: pibs
 */

#include <sysBase/gpio.h>
#include <sysBase/pwm.h>
#include <sysBase/usartx.h>


#ifndef CONFIG_H_
#define CONFIG_H_


#define MINIMAL_TIME_MS		1	// tempo minimo do millis()
#define INIT_CONTER		0	// iniciar contagem do milles em zero

#define BAUDRATE		19200
#define USART_DBG		USART3

#define LED			GPIOC13
#define TIME_BLK		500	// tempo em miles segundos para piscadas do les 13

#define S_FRONT			GPIOA0
#define S_BEHIND		GPIOA1
#define S_LEFT			GPIOA2
#define S_RIGHT			GPIOA3
#define S_BLEFT			GPIOA4
#define S_BRIGHT		GPIOA5
#define CH_FRONT		ADC_CH0
#define CH_BEHIND		ADC_CH1
#define CH_LEFT			ADC_CH2
#define CH_RIGHT		ADC_CH3
#define CH_BLEFT		ADC_CH4
#define CH_BRIGHT		ADC_CH5

#define CLT_PH			GPIOB14

#define CLT_MA			GPIOA10
#define PIN_MA1			GPIOB15
#define PIN_MA2			GPIOA8

#define CLT_MB			GPIOA9
#define PIN_MB1			GPIOB12
#define PIN_MB2			GPIOB13

#define TURN_BACK		0
#define TURN_FRONT		1
#define TURN_OFF		4


#endif /* CONFIG_H_ */
