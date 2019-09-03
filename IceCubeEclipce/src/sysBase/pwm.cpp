/*
 * pwm.cpp
 *
 *  Created on: 13 de set de 2018
 *      Author: pibs
 */

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <../include/sysBase/gpio.h>
#include <../include/sysBase/pwm.h>
#include <system_stm32f10x.h>


/* Private macro -------------------------------------------------------------*/


// CAN BE CHANGED
#define TIM_PWM1	TIM1			// ao alterar o timer cuide de utilizara a função
#define PERIPH_PWM1	RCC_APB2Periph_TIM1	// do baramento respequitivo e os PIN_PWMx

#define PIN_PWM1	GPIOA8			// pino que corresondos a saidas do canal do
#define PIN_PWM2	GPIOA9			// timer utilisado
#define PIN_PWM3	GPIOA10			// se o timer dor alterado esse valores consequentemente
#define PIN_PWM4	GPIOA11			// tabem serão alterados

/* Private variables ---------------------------------------------------------*/



/* Private Functions -----------------------------------------------------------------*/



/*****************************************************************************
*  Configura a frwquencia de trabalho do pwm o prescaler, e period dvem ser
*  combinados afim de estabelecer a resoluçãoe frequencias adequadas.
*
*  EX: pwm_SetFreg(4096000, 4096); define uma frequencia de 1 kHz com uma
*  resoloção de 0 a 4096 12 bits
******************************************************************************/
void pwm_SetFreg(uint32_t prescaler, uint16_t period){

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	TIM_TimeBaseStructInit(&timerInitStructure);

	timerInitStructure.TIM_Prescaler		= ( SystemCoreClock / prescaler ) - 1;
	timerInitStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	timerInitStructure.TIM_Period			= period;
	timerInitStructure.TIM_ClockDivision		= TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter	= 0;

	TIM_DeInit(TIM_PWM1);
	RCC_APB1PeriphClockCmd(PERIPH_PWM1, ENABLE);
	TIM_TimeBaseInit(TIM_PWM1, &timerInitStructure);
	TIM_Cmd(TIM_PWM1, ENABLE);
}

/*****************************************************************************
*  desabilita todos pwm e periferia envolvita com exeção da GPIO.
*  O GPIO mantem as configuações iniciasas na função:
*  	pwm_OnOffCh(uint8_t ch, uint8_t state);
******************************************************************************/
void pwm_DisableALL(void){
	TIM_OC1PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
	TIM_OC4PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
	TIM_ClearITPendingBit(TIM_PWM1, TIM_IT_Update);
	TIM_Cmd(TIM_PWM1, DISABLE);
	RCC_APB1PeriphClockCmd(PERIPH_PWM1, DISABLE);
}


/*****************************************************************************
* Inicia o canal PWM nolmamente deslicadoe com a GPIO correspondente
* devifamente configurada
******************************************************************************/
void pwm_OnOffCh(uint8_t ch, uint8_t state){
	TIM_OCInitTypeDef outputChannelInit 	= {0,0,0,0,0,0,0,0};
	outputChannelInit.TIM_OCMode 		= TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse 		= 0;
	outputChannelInit.TIM_OutputState 	= TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity 	= TIM_OCPolarity_High;
	state 					= (state) ? TIM_OCPreload_Enable: TIM_OCPreload_Disable;

	switch(ch){
		case 1:
			gpio_Mode(PIN_PWM1, GPIO_Mode_AF_PP);
			TIM_OC1Init(TIM_PWM1, &outputChannelInit);
			TIM_OC1PreloadConfig(TIM_PWM1, state);

			break;
		case 2:
			gpio_Mode(PIN_PWM2, GPIO_Mode_AF_PP);
			TIM_OC2Init(TIM_PWM1, &outputChannelInit);
			TIM_OC2PreloadConfig(TIM_PWM1, state);
			break;
		case 3:
			gpio_Mode(PIN_PWM3, GPIO_Mode_AF_PP);
			TIM_OC3Init(TIM_PWM1, &outputChannelInit);
			TIM_OC3PreloadConfig(TIM_PWM1, state);
			break;
		case 4:
			gpio_Mode(PIN_PWM4, GPIO_Mode_AF_PP);
			TIM_OC4Init(TIM_PWM1, &outputChannelInit);
			TIM_OC4PreloadConfig(TIM_PWM1, state);

			break;
	}
}

/*****************************************************************************
 * iconfigura o duty cycle do canal pwm, de PWM_CHx, value segue a resolução
 * configurada na função:
 * 	void pwm_SetFreg(uint32_t prescaler, uint16_t period)
******************************************************************************/
void pwm_SetCh(uint8_t ch, uint16_t dutyCycleON){
	TIM_OCInitTypeDef outputChannelInit	= {0,0,0,0,0,0,0,0};
	outputChannelInit.TIM_OCMode 		= TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse 		= dutyCycleON;
	outputChannelInit.TIM_OutputState 	= TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity 	= TIM_OCPolarity_High;

	switch(ch){
		case 1:
			TIM_OC1Init(TIM_PWM1, &outputChannelInit);
			TIM_OC1PreloadConfig(TIM_PWM1, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(TIM_PWM1, &outputChannelInit);
			TIM_OC2PreloadConfig(TIM_PWM1, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(TIM_PWM1, &outputChannelInit);
			TIM_OC3PreloadConfig(TIM_PWM1, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(TIM_PWM1, &outputChannelInit);
			TIM_OC4PreloadConfig(TIM_PWM1, TIM_OCPreload_Enable);
			break;
		default:
			TIM_OC1PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
			TIM_OC2PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
			TIM_OC3PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
			TIM_OC4PreloadConfig(TIM_PWM1, TIM_OCPreload_Disable);
	}
}






