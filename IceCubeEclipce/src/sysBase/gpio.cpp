/*
 * gpio.cpp
 *
 *  Created on: 5 de set de 2018
 *      Author: pibs
 */

#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_rcc.h>
#include <../include/sysBase/gpio.h>
#include <../include/sysBase/iwdg.h>
#include <../include/sysBase/utility_timer.h>



/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
typedef struct{
	GPIO_TypeDef 	*GPIOx;								// GPIO PORT
	uint16_t	PINx;								// GPIO PIN
}gpioPin;

bool pogFdp;



/* Private Functions ---------------------------------------------------------*/
gpioPin gpio_Translator(uint8_t pin);							// a depender do MCU essa funçãodeve ser
											// retrabalhada de forma a atender
											// as especificações fisicas do mesmo


/*****************************************************************************
 * Inicia toas as periferias de GPIO em mode entrada de alta inpedancia
******************************************************************************/
void gpio_initAll(bool dbg){								// ENABLE bus GPIO
	GPIO_InitTypeDef gpioMode;

	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_AFIODeInit();
	if(dbg == false){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		delay_ms(5000);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
		delay_ms(1000);
	}
	GPIO_StructInit(&gpioMode);

	GPIO_Init(GPIOA, &gpioMode);
	GPIO_Init(GPIOB, &gpioMode);
	GPIO_Init(GPIOC, &gpioMode);
	GPIO_Init(GPIOD, &gpioMode);
}

/*****************************************************************************
 * Configura o modo dr trabalho do pino emtre emtada e saida, anbos NA ou NF
******************************************************************************/
void gpio_Mode(uint8_t pin, GPIOMode_TypeDef mode){					// SET MODE PIN OPERATION
	GPIO_InitTypeDef gpioMode;
	gpioPin gpioAUX;

	gpioAUX = gpio_Translator(pin);

	gpioMode.GPIO_Pin	= gpioAUX.PINx;
	gpioMode.GPIO_Mode	= mode;
	gpioMode.GPIO_Speed	= GPIO_Speed_2MHz;

	GPIO_Init(gpioAUX.GPIOx, &gpioMode);
}
/*****************************************************************************
 * Lé o estado de mum pino, apenas posivel ler pinos previamente configurados
******************************************************************************/
bool gpio_Read(uint8_t pin){
	gpioPin gpioAUX;
	gpioAUX = gpio_Translator(pin);
	if(GPIO_ReadInputDataBit(gpioAUX.GPIOx, gpioAUX.PINx) == Bit_SET) return true;
	return false;
}

/*****************************************************************************
 * escreve um valor boleano em mu pino, apenas posivel escrever pinos
 * devidaemte configurados
******************************************************************************/
void gpio_Write(uint8_t pin, bool value){
	gpioPin gpioAUX;
	gpioAUX = gpio_Translator(pin);
	GPIO_WriteBit(gpioAUX.GPIOx, gpioAUX.PINx, (BitAction)value);
}


void gpio_InitInterrupt(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource,  EXTITrigger_TypeDef mode){
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	switch(GPIO_PinSource){
		case GPIO_PinSource0:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI0_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line0;
			break;

		case GPIO_PinSource1:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI1_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line1;
			break;

		case GPIO_PinSource2:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI2_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line2;
			break;

		case GPIO_PinSource3:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI3_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line3;
			break;

		case GPIO_PinSource4:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI4_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line4;
			break;

		case GPIO_PinSource5:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI9_5_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line5;
			break;

		case GPIO_PinSource6:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI9_5_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line6;
			break;

		case GPIO_PinSource7:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI9_5_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line7;
			break;

		case GPIO_PinSource8:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI9_5_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line8;
			break;

		case GPIO_PinSource9:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI9_5_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line9;
			break;

		case GPIO_PinSource10:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI15_10_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line10;
			break;

		case GPIO_PinSource11:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI15_10_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line11;
			break;

		case GPIO_PinSource12:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI15_10_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line12;
			break;

		case GPIO_PinSource13:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI15_10_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line13;
			break;

		case GPIO_PinSource14:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI15_10_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line14;
			break;

		case GPIO_PinSource15:
			NVIC_InitStructure.NVIC_IRQChannel	= EXTI15_10_IRQn;
			EXTI_InitStruct.EXTI_Line		= EXTI_Line15;
			break;

		default: return;
	}


	EXTI_InitStruct.EXTI_LineCmd				= ENABLE;
	EXTI_InitStruct.EXTI_Mode				= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger				= mode;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0x00;			//Priority 2,
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0x00;			//Sub priority 2
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;		//Enable external interrupt channel

	EXTI_Init(&EXTI_InitStruct);
	NVIC_Init(&NVIC_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);
}


/* Private Functions ---------------------------------------------------------*/
/*****************************************************************************
 * tradus o label do pino para GPIOx, GPIO_Pin_x correspondente.
 * a lebel está acociada ao numero do pino fisico do MUC utilizado
******************************************************************************/
gpioPin gpio_Translator(uint8_t pin){
	gpioPin		gpio;								// TRANSLATOR HARDWARE PIN TO GPIO
	switch(pin){
		//=========================== A ============================//
		case 10: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_0;	break;
		case 11: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_1;	break;
		case 12: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_2;	break;
		case 13: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_3;	break;
		case 14: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_4; 	break;
		case 15: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_5; 	break;
		case 16: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_6; 	break;
		case 17: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_7; 	break;
		case 29: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_8; 	break;
		case 30: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_9; 	break;
		case 31: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_10;	break;
		case 32: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_11;	break;
		case 33: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_12;	break;
		case 34: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_13;	break;
		case 37: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_14;	break;
		case 38: gpio.GPIOx = GPIOA; gpio.PINx = GPIO_Pin_15;	break;
		//=========================== B ============================//
		case 18: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_0;	break;
		case 19: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_1;	break;
		case 20: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_2;	break;
		case 21: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_10;	break;
		case 22: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_11;	break;
		case 25: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_12;	break;
		case 26: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_13;	break;
		case 27: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_14;	break;
		case 28: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_15;	break;
		case 39: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_3;	break;
		case 40: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_4;	break;
		case 41: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_5;	break;
		case 42: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_6;	break;
		case 43: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_7;	break;
		case 45: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_8;	break;
		case 46: gpio.GPIOx = GPIOB; gpio.PINx = GPIO_Pin_9;	break;
		//=========================== C ============================//
		case 2: gpio.GPIOx = GPIOC; gpio.PINx = GPIO_Pin_13;	break;
		case 3: gpio.GPIOx = GPIOC; gpio.PINx = GPIO_Pin_14;	break;
		case 4: gpio.GPIOx = GPIOC; gpio.PINx = GPIO_Pin_15;	break;
		//=========================== D ============================//
		case 5: gpio.GPIOx = GPIOD; gpio.PINx = GPIO_Pin_0;	break;
		case 6: gpio.GPIOx = GPIOD; gpio.PINx = GPIO_Pin_1;	break;
	}
	return gpio;
}

