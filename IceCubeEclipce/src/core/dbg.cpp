/*
 * dbg.cpp
 *
 *  Created on: 17 de mai de 2019
 *      Author: pibs
 */

#include <core/config.h>
#include <core/protocol.h>
#include <stm32f10x_rtc.h>
#include <sysBase/usartx.h>
#include <cstdlib>



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char bffRtc[11];

/* Private Functions ---------------------------------------------------------*/
void printRTC(void);
void printLabel(CCHR *label);

/*******************************************************************************
 * enviar informações para seria do dbg
 * printDbg(NULL, "reiniciando..."); 	-> [0000000042] sistema reiniciando...
 * printDbg("taxa", "555");		-> [0000000042] taxa: 555
********************************************************************************/
void printDbgSTR(CCHR *label, CCHR *str){
	printRTC();
	printLabel(label);
	usart_SendStrLn(USART_DBG, str);
}

/*******************************************************************************
 * enviar informações para seria do dbg
 * printDbg(NULL, 2565);		-> [0000000042] 2565
 * printDbg("taxa", 2565);		-> [0000000042] taxa: 2565
********************************************************************************/
void printDbgU32(CCHR *label, const u32 value){
	printRTC();
	printLabel(label);
	itoa(value, bffRtc, 10);
	usart_SendStrLn(USART_DBG, bffRtc);
}



/*******************************************************************************
 * str é a estring a ser enviada, e endChrar é o final
 * para pular linha o final deve ser '\n'
*******************************************************************************/
void serialPrint(CCHR *str, CCHR endChrar){
	usart_SendStr(USART_DBG, str);
	usart_SendChr(USART_DBG, endChrar);
}



/*########################################################################################################################################################*/
/*########################################################################################################################################################*/
/*########################################################################################################################################################*/
/* Private Functions ---------------------------------------------------------*/

/*******************************************************************************
 * imprime para o dbg o valor do rtc
 * [0000000042]
********************************************************************************/
void printRTC(void){
	itoa(RTC_GetCounter(), bffRtc, 10);
	zeroLeft(bffRtc, 10);
	usart_SendChr(USART_DBG, '[');
	usart_SendStr(USART_DBG, bffRtc);
	usart_SendChr(USART_DBG, ']');
	usart_SendChr(USART_DBG, ' ');
}


/*******************************************************************************
 * imprime para o dbg uma label se não for nula -> "label: "
********************************************************************************/
void printLabel(CCHR *label){
	if(label != NULL){
		usart_SendStr(USART_DBG, label);
		usart_SendChr(USART_DBG, ':');
		usart_SendChr(USART_DBG, ' ');
	}
}



