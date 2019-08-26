/*
 * iwdg.cpp
 *
 *  Created on: 13 de set de 2018
 *      Author: pibs
 */



#include <stm32f10x_iwdg.h>
#include <../include/sysBase/iwdg.h>


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private Functions -----------------------------------------------------------------*/


/*****************************************************************************
 * esponsavel por reiniciar o micro de forma dorçasa caso uma trava ocorra
 * um contador deve ser resetado (reiniciado) do contrario o MCU reiniciará
 *
 * clok iwdg = 40khz
 * topCounter: 0 - 4096
 * Prescaler IWDG_Prescaler_4: IWDG prescaler set to 4
 * Prescaler IWDG_Prescaler_8: IWDG prescaler set to 8
 * Prescaler IWDG_Prescaler_16: IWDG prescaler set to 16
 * Prescaler IWDG_Prescaler_32: IWDG prescaler set to 32
 * Prescaler IWDG_Prescaler_64: IWDG prescaler set to 64
 * Prescaler IWDG_Prescaler_128: IWDG prescaler set to 128
 * Prescaler IWDG_Prescaler_256: IWDG prescaler set to 256
******************************************************************************/
void iwdg_Init(uint16_t topCounter, uint8_t Prescaler){
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);			// habilitar acesso as configurações
	IWDG_SetPrescaler(Prescaler);					// prscalar
	IWDG_SetReload(topCounter);					// valor de extrouro
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);			// desabilitar acesso as configurações
	IWDG_Enable();							// habilitar iwdg
}


/*****************************************************************************
 * reinicia o contador do iwdg
******************************************************************************/
void iwdg_RstCont(void){							// reiniciar contagam do iwdg
	IWDG_ReloadCounter();
}
