/*
 * usartx.h
 *
 *  Created on: 6 de set de 2018
 *      Author: pibs
 */

#ifndef USARTX_H_
#define USARTX_H_

#include <stdint.h>
#include <stm32f10x.h>



#define BFF1		0				// buffer um
#define BFF2		1				// buffer dois
#define BFF3		2				// buferr trez
#define N_BFF		3				// quantidade de buffer 55
#define SIZE_BFF	512				// tamanho limite do buffer


typedef struct{						// tipo buffer para operar con toras as usart em um unico pacote
	volatile char		data[SIZE_BFF];
	volatile uint16_t	cunter;
}buffer;


// controle dos perifericos
void usart1_Setup(uint32_t BaudRate);
void usart2_Setup(uint32_t BaudRate);
void usart3_Setup(uint32_t BaudRate);


// conteole do buffer
char *usart_GetBFF(uint8_t bf);
uint16_t usart_SizeBFF(uint8_t bf);
void usart_CleanBFF(uint8_t bf);


// contele de comunicação
void usart_SendStr(USART_TypeDef* USARTx, const char *str);
void usart_SendChr(USART_TypeDef* USARTx, const char chr);
void usart_SendStrLn(USART_TypeDef* USARTx, const char *str);


#endif /* USARTX_H_ */
