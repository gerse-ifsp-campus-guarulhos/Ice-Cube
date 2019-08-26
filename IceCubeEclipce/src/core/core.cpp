/*
 * core.cpp
 *
 *  Created on: 14 de ago de 2019
 *      Author: pibs
 */

#include <core/config.h>
#include <core/pogPWM.h>
#include <core/protocol.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_iwdg.h>
#include <sysBase/adc.h>
#include <sysBase/gpio.h>
#include <sysBase/iwdg.h>
#include <sysBase/pwm.h>
#include <sysBase/usartx.h>
#include <sysBase/utility_timer.h>

/* Private macro ---------------------------------------------------------------------------------------------------------------------------------------- */

/* Private variables ------------------------------------------------------------------------------------------------------------------------------------ */
static bool blink = false;
static u64 time = 0;
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */

/*******************************************************************************
 * inicia e configura o hardware de acordo com a placa utilizada
*******************************************************************************/
void startUpHardware(void){
	/* iniciar temporezadores */
	write_counter(INIT_CONTER);				// iniciar contador
	init_timer_interrupt(MINIMAL_TIME_MS);			// intervalo minimo do tempo para delay/millis
	init_rtc();						// iniiar relogio interno

	gpio_initAll(false);					// iniciar gpios sem dbg
	adc1_Init();						// iniciar adcs

//	iniciando POG PWM
	initPWM();
	pwmENABLE(true);

//	saida
	gpio_Mode(LED, GPIO_Mode_Out_PP);

//	saida CLT motor
	gpio_Mode(CLT_PH, GPIO_Mode_Out_PP);
	gpio_Mode(PIN_MA1, GPIO_Mode_Out_PP);
	gpio_Mode(PIN_MA2, GPIO_Mode_Out_PP);
	gpio_Mode(PIN_MB1, GPIO_Mode_Out_PP);
	gpio_Mode(PIN_MB2, GPIO_Mode_Out_PP);


//	entrada
	gpio_Mode(S_FRONT, GPIO_Mode_AIN);
	gpio_Mode(S_BEHIND, GPIO_Mode_AIN);
	gpio_Mode(S_LEFT, GPIO_Mode_AIN);
	gpio_Mode(S_RIGHT, GPIO_Mode_AIN);
	gpio_Mode(S_BLEFT, GPIO_Mode_AIN);
	gpio_Mode(S_BRIGHT, GPIO_Mode_AIN);

	/* iniciar cão de garda para previnir loops	*/
	/* infinitos 					*/
	iwdg_Init(4095, IWDG_Prescaler_8);

	usart3_Setup(BAUDRATE);
}



/*******************************************************************************
 * monitor basico do sistema
*******************************************************************************/
void coreMonitor(void){
	delay_ms(5);					// estabilizar maximo 5 minimo 1
	iwdg_RstCont();					// cão de garda
}


/*******************************************************************************
 * para ler os sensores 0 a 4096
*******************************************************************************/
u16 sLeft(void){
	return adc1_Read(CH_LEFT);
}

u16 sRight(void){
	return adc1_Read(CH_RIGHT);
}

u16 sFront(void){
	return adc1_Read(CH_FRONT);
}

u16 sBehind(void){
	return adc1_Read(CH_BEHIND);
}

u16 sbBright(void){
	return adc1_Read(CH_BRIGHT);
}

u16 sbLeft(void){
	return adc1_Read(CH_BLEFT);
}

/*******************************************************************************
 * clt ponte H
*******************************************************************************/
void cltPonteH(bool stop){
	gpio_Write(CLT_PH, stop);
}


/*******************************************************************************
 * clt motor A dcly 0 a DCLY_T
 * comandos posiveis TURN_BACK , TURN_FRONT, TURN_OFF
*******************************************************************************/
void cltMA(u16 dcly, u8 turn){
	setDclyMA(dcly);
	switch(turn){
		case TURN_BACK:
			gpio_Write(PIN_MA1, true);
			gpio_Write(PIN_MA2, false);
			break;

		case TURN_FRONT:
			gpio_Write(PIN_MA1, false);
			gpio_Write(PIN_MA2, true);
			break;

		case TURN_OFF:
		default:
			setDclyMA(0);
			gpio_Write(PIN_MA1, false);
			gpio_Write(PIN_MA2, false);
			break;
	}
}


/*******************************************************************************
 * clt motor b dcly 0 a DCLY_T
 * comandos posiveis TURN_BACK , TURN_FRONT, TURN_OFF
*******************************************************************************/
void cltMB(u16 dcly, u8 turn){
	setDclyMB(dcly);
	switch(turn){
		case TURN_BACK:
			gpio_Write(PIN_MB1, true);
			gpio_Write(PIN_MB2, false);
			break;

		case TURN_FRONT:
			gpio_Write(PIN_MB1, false);
			gpio_Write(PIN_MB2, true);
			break;

		case TURN_OFF:
		default:
			setDclyMB(0);
			gpio_Write(PIN_MB1, false);
			gpio_Write(PIN_MB2, false);
			break;
	}
}


/*******************************************************************************
 * pesca o led 13 em um sinal quadratico de TIME_BLK [ms], essa função serve
 * para debugar o codigo se algo travar o mcu por muito mais tempo que TIME_BLK
 * será percepitivel a olho nú
*******************************************************************************/
void blkLed(void){
	if(millis() > time){
		blink = not blink;
		gpio_Write(LED, blink);
		time  = millis() + TIME_BLK;
	}
}


/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */


