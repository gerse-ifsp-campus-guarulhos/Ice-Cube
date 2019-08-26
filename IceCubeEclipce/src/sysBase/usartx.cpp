// -------------------------------------------------------------------
//   Copyright (C) Pedro Igor B. S 2018
// -------------------------------------------------------------------
//   Descrição: Esse progrma é uma bibliotéca para facilitar o uso do
//   periferios do microcontrolador STM32F10x
//   Autores: Pedro Igor Borçatti da Silva
//
//
//   Licença: GNU GPL 2
// -------------------------------------------------------------------
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; version 2 of the
//   License.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// ------------------------------------------------------------------

#include <misc.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <stdint.h>
#include <../include/sysBase/usartx.h>





/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static volatile buffer bff[N_BFF];
static volatile char bffAUX = 0x00;


/* Private Functions ---------------------------------------------------------*/
void makeBff(USART_TypeDef* USARTx, uint8_t bf);					// capturar buffer



/*******************************************************************************
 * configura e inicia a porta serial em modo assincrono com a velocidade
 * BaudRate, os pinos são configurados altomaticamente por essa função
 * dispensando configurações adicionaes.
********************************************************************************/
void usart1_Setup(uint32_t BaudRate){
	usart_CleanBFF(BFF1);
	/**** Enable peripheral clocks for USARTx on GPIOB ****/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,	ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,		ENABLE);

	/* Configure PB7 and PB6 as USART1 TX/RX */
	/* PB6 = alternate function push/pull output */

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin			= GPIO_Pin_6;				// TX pin
	gpio.GPIO_Speed			= GPIO_Speed_50MHz;			// speed pin
	gpio.GPIO_Mode			= GPIO_Mode_AF_PP;			// push up
	GPIO_Init(GPIOB, &gpio);
	/* PB7 = floating input */
	gpio.GPIO_Pin			= GPIO_Pin_7;				// RX pin
	gpio.GPIO_Mode			= GPIO_Mode_IN_FLOATING;		// ploating point
	GPIO_Init(GPIOB, &gpio);

	/* Configure and initialize usart... */
	USART_InitTypeDef usart;
	usart.USART_BaudRate 		= BaudRate;
	usart.USART_WordLength		= USART_WordLength_8b;			// length framer
	usart.USART_StopBits		= USART_StopBits_1;
	usart.USART_Parity		= USART_Parity_No;
	usart.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	usart.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;	// mode full duplex

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel 			= USART1_IRQn;	//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;		//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;	//

	USART_Init(USART1, &usart);
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

/*******************************************************************************
 * configura e inicia a porta serial em modo assincrono com a velocidade
 * BaudRate, os pinos são configurados altomaticamente por essa função
 * dispensando configurações adicionaes.
********************************************************************************/
void usart2_Setup(uint32_t BaudRate){
	usart_CleanBFF(BFF2);
	/**** Enable peripheral clocks for USARTx on GPIOA ****/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,	ENABLE);

	/* Configure PA9 and PA10 as USART1 TX/RX */
	/* PA9 = alternate function push/pull output */
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin			= GPIO_Pin_2;				// TX pin
	gpio.GPIO_Speed			= GPIO_Speed_50MHz;			// speed pin
	gpio.GPIO_Mode			= GPIO_Mode_AF_PP;			// push up
	GPIO_Init(GPIOA, &gpio);
	/* PA10 = floating input */
	gpio.GPIO_Pin			= GPIO_Pin_3;				// RX pin
	gpio.GPIO_Mode			= GPIO_Mode_IN_FLOATING;		// ploating point
	GPIO_Init(GPIOA, &gpio);

	/* Configure and initialize usart... */
	USART_InitTypeDef usart;
	usart.USART_BaudRate 		= BaudRate;
	usart.USART_WordLength		= USART_WordLength_8b;			// length framer
	usart.USART_StopBits		= USART_StopBits_1;
	usart.USART_Parity		= USART_Parity_No;
	usart.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	usart.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;	// mode full duplex

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel 			= USART2_IRQn;	//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;		//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;	//

	USART_Init(USART2, &usart);
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

/*******************************************************************************
 * configura e inicia a porta serial em modo assincrono com a velocidade
 * BaudRate, os pinos são configurados altomaticamente por essa função
 * dispensando configurações adicionaes.
********************************************************************************/
void usart3_Setup(uint32_t BaudRate){
	usart_CleanBFF(BFF3);
	/**** Enable peripheral clocks for USARTx on GPIOA ****/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,	ENABLE);

	/* Configure PA9 and PA10 as USART1 TX/RX */
	/* PA9 = alternate function push/pull output */
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin			= GPIO_Pin_10;				// TX pin
	gpio.GPIO_Speed			= GPIO_Speed_50MHz;			// speed pin
	gpio.GPIO_Mode			= GPIO_Mode_AF_PP;			// push up
	GPIO_Init(GPIOB, &gpio);
	/* PA10 = floating input */
	gpio.GPIO_Pin			= GPIO_Pin_11;				// RX pin
	gpio.GPIO_Mode			= GPIO_Mode_IN_FLOATING;		// ploating point
	GPIO_Init(GPIOB, &gpio);

	/* Configure and initialize usart... */
	USART_InitTypeDef usart;
	usart.USART_BaudRate 		= BaudRate;
	usart.USART_WordLength		= USART_WordLength_8b;			// length framer
	usart.USART_StopBits		= USART_StopBits_1;
	usart.USART_Parity		= USART_Parity_No;
	usart.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	usart.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;	// mode full duplex

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel 			= USART3_IRQn;	//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;		//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;	//

	USART_Init(USART3, &usart);
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}






/*******************************************************************************
 * retorna o endereço do array de char do tipo buffer corrspondente a USART
 * USART1 BFF1
 * USART2 BFF2
 * USART4 BFF3
 * O bufffer é montado pela interrupção espequitiva ao prefiferio, sendo
 * contruido byte a byte assim que o mesmo termina se ser recebido
********************************************************************************/
char *usart_GetBFF(uint8_t bf){
	return (char*)bff[bf].data;
}

/*******************************************************************************
 * retorna a quantidades de bytes presentes no buffer corrspondente a USART
********************************************************************************/
uint16_t usart_SizeBFF(uint8_t bf){
	return bff[bf].cunter;
}

/*******************************************************************************
 * limpa com 0 buffer corrspondente a USART
********************************************************************************/
void usart_CleanBFF(uint8_t bf){
	bff[bf].cunter = 0;
	for(uint16_t i=0; i<SIZE_BFF; i++ ){
		bff[bf].data[1] = 0;
	}
}




/*******************************************************************************
 * Envia uma string ou conjunto de bytes por uma USART (USART1, USART2 .....)
 * o envio termina no valor 0x00
*******************************************************************************/
void usart_SendStr(USART_TypeDef* USARTx, const char *str){
	do{
		USART_SendData(USARTx, *str++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}while(*str);
}

/*******************************************************************************
 * Envia uma string ou conjunto de bytes por uma USART (USART1, USART2 .....)
 * o envio termina no valor 0x00, e apos isso um \n é enviado
********************************************************************************/
void usart_SendStrLn(USART_TypeDef* USARTx, const char *str){
	do{
		USART_SendData(USARTx, *str++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}while(*str);
	usart_SendChr(USARTx, '\n');
}

/*******************************************************************************
 * Envia um char ou byte por uma USART (USART1, USART2 .....)
********************************************************************************/
void usart_SendChr(USART_TypeDef* USARTx, const char chr){
		USART_SendData(USARTx, chr);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}


/* Private Functions ---------------------------------------------------------*/

/*******************************************************************************
 * contrutor do buffer capitura o byte presente e o conta em bff.coter
********************************************************************************/
void makeBff(USART_TypeDef* USARTx, uint8_t bf){
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) == RESET ) return;
	bffAUX = 0x00;
	switch (bf) {
		case BFF1:
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			bffAUX = (char)USART_ReceiveData(USART1);
			break;
		case BFF2:
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			bffAUX = (char)USART_ReceiveData(USART2);
			break;
		case BFF3:
			USART_ClearITPendingBit(USART3, USART_IT_RXNE);
			bffAUX = (char)USART_ReceiveData(USART3);
			break;
	}

	// se o buffer estiver cheio descarte iginore a chamada e descarte os proximos bytes
	if(bff[bf].cunter >= SIZE_BFF ) return;
	bff[bf].data[bff[bf].cunter++]	= bffAUX;
	bff[bf].data[bff[bf].cunter]	= 0x00;

	//bff[BFF_DBUG].data[bff[BFF_DBUG].cunter++] = bffAUX;
	//bff[BFF_DBUG].data[bff[BFF_DBUG].cunter] = 0x00;

}

/*******************************************************************************
 * interrupções das usart
********************************************************************************/
extern "C" {
	void USART1_IRQHandler(void){
		makeBff(USART1, BFF1);
	}

	void USART2_IRQHandler(void){
		makeBff(USART2, BFF2);
	}

	void USART3_IRQHandler(void){						// GPRS
		makeBff(USART3, BFF3);
	}
}




