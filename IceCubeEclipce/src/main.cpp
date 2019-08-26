/*
 * main.cpp
 *
 *  Created on: 13 de ago de 2019
 *      Author: gerse
 */

#include <core/config.h>
#include <core/core.h>
#include <core/pogPWM.h>
#include <core/dbg.h>
#include <core/protocol.h>
#include <stddef.h>
#include <sysBase/utility_timer.h>


/* Private macro ---------------------------------------------------------------------------------------------------------------------------------------- */
#define TIME_S_READ		100	// tempo em miles segundos

/* Private variables ------------------------------------------------------------------------------------------------------------------------------------ */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */


/*******************************************************************************
 * FAZER CONFIG ADICIONAS AQUI
*******************************************************************************/
void setUp(void){
	cltPonteH(true);
	printDbgSTR(NULL, "INIT... OK");
}


/*******************************************************************************
 * FAZER O PROGRAMA PRINCIPAL AQUI...
*******************************************************************************/
void loop(void){
	static u64 time = millis() + TIME_S_READ;
	static u16 pwm = 0;

	if(time < millis()){
		printDbgU32("sLeft",	sLeft());
		printDbgU32("sRight",	sRight());
		printDbgU32("sFront",	sFront());
		printDbgU32("sBehind",	sBehind());
		printDbgU32("sbBright",	sbBright());
		printDbgU32("sbLeft",	sbLeft());

		cltMA(pwm, TURN_BACK);
		cltMB(pwm, TURN_FRONT);
		printDbgU32("pwm", pwm++);

		if(pwm>DCLY_T) pwm = 0;

		time = millis() + TIME_S_READ;
	}
}



/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */


int main(void){
	startUpHardware();	setUp();			// setup - config
	do{ blkLed();	loop(); coreMonitor(); }while(true); 	// loop principal
	return 0;
}

