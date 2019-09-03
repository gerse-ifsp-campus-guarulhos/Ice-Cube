/*
 * pogPWM.cpp
 *
 *  Created on: 21 de ago de 2019
 *      Author: pibs
 */


#include <core/protocol.h>
#include <core/config.h>
#include <core/core.h>
#include <core/pogPWM.h>
#include <sysBase/gpio.h>
#include <sysBase/utility_timer.h>



/* Private macro ---------------------------------------------------------------------------------------------------------------------------------------- */
/* Private variables ------------------------------------------------------------------------------------------------------------------------------------ */
bool pwmEnable	= false;
u16 freg	= 0;
u16 newMa	= 0;
u16 newMb	= 0;
u16 ma		= 0;
u16 mb		= 0;



/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */
void selectBordA(void);
void selectBordB(void);
void attDcly(void);

/*******************************************************************************
 * essa função inicia o motor do pwm e configura as saidas respectivas
*******************************************************************************/
void initPWM(void){
	freg	= 0;
	ma	= 0;
	mb	= 0;
	newMa	= 0;
	newMb	= 0;

	gpio_Mode(CLT_MA, GPIO_Mode_Out_PP);
	gpio_Mode(CLT_MB, GPIO_Mode_Out_PP);
	gpio_Write(CLT_MA, false);
	gpio_Write(CLT_MB, false);
	set_counter(PERIOD);
}

/*******************************************************************************
 * essa função é o motor pog do pwm
*******************************************************************************/
void pwmENABLE(bool enable){
	if(enable){
		pwmEnable = true;
	} else {
		pwmEnable = false;
		freg = 0;
		gpio_Write(CLT_MA, false);
		gpio_Write(CLT_MB, false);
	}
}

/*******************************************************************************
 * essa função é o motor pog do pwm
*******************************************************************************/
void pwmLoop(void){
	if(pwmEnable == false) return;

	selectBordA();
	selectBordB();

	if(++freg >=  DCLY_T) attDcly();
}


/*******************************************************************************
 * essa função é o motor pog do pwm 0 a DCLY_T
*******************************************************************************/
void setDclyMA(u16 dcly){
	newMa = (dcly >= DCLY_T)?(DCLY_T):(dcly);
}

/*******************************************************************************
* essa função é o motor pog do pwm 0 a DCLY_T
*******************************************************************************/
void setDclyMB(u16 dcly){
	newMb = (dcly >= DCLY_T)?(DCLY_T):(dcly);
}

/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */


/*******************************************************************************
 *  essa função controla o ciclo de borda do pwm
*******************************************************************************/
void selectBordA(void){
	if(ma > freg){
		gpio_Write(CLT_MA, true);
	} else {
		gpio_Write(CLT_MA, false);
	}
}

/*******************************************************************************
 *  essa função controla o ciclo de borda do pwm
*******************************************************************************/
void selectBordB(void){
	if(mb > freg){
		gpio_Write(CLT_MB, true);
	} else {
		gpio_Write(CLT_MB, false);
	}
}

void attDcly(void){
	freg	= 0;
	ma	= newMa;
	mb	= newMb;
}
