/*
 * IceCube.cpp
 *
 *  Created on: 1 de set de 2019
 *      Author: pibs
 */

#include <core/config.h>
#include <core/core.h>
#include <core/pogPWM.h>
#include <core/dbg.h>
#include <core/protocol.h>
#include <stddef.h>
#include <sysBase/utility_timer.h>


/* Private macro ---------------------------------------------------------------------------------------------------------------------------------------- */
#define TIME_S_READ		500	// tempo em miles segundos
#define R_MIN			700	//
#define R_MID			1000	//
#define R_MAX			4095	//

#define V_MAX			499	//
#define V_MID			400	//
#define V_MIN			350	//

/* Private variables ------------------------------------------------------------------------------------------------------------------------------------ */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */
void empurrar(void);
void travarL(void);
void travarR(void);
void dbg(void);

/*******************************************************************************
 * FAZER CONFIG ADICIONAS AQUI
*******************************************************************************/
void setUp(void){
	cltPonteH(true);
	cltMA(0, TURN_OFF);
	cltMB(0, TURN_OFF);

	printDbgSTR(NULL, "INIT... OK");




}


/*******************************************************************************
 * FAZER O PROGRAMA PRINCIPAL AQUI...
*******************************************************************************/
void loop(void){
	empurrar();
	dbg();
}




/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */

/* enpura e resolve mira */
void empurrar(void){
	u16 value = sFront();
	if((value > R_MIN) &&  (value < R_MID)){
		cltMA(V_MID, TURN_FRONT);
		cltMB(V_MID, TURN_FRONT);
	} else if(value > R_MID){
		cltMA(V_MAX, TURN_FRONT);
		cltMB(V_MAX, TURN_FRONT);
	} else {
		travarL();
		travarR();
	}
}

/* trava o alvo caso saia da mira, direira para esquerda  */
void travarL(void){
	u16 value = sLeft();
	if((value > R_MIN) &&  (value < R_MID)){
		cltMA(V_MIN, TURN_FRONT);
		cltMB(V_MIN, TURN_BACK);
	} else if(value > R_MID){
		cltMA(V_MAX, TURN_FRONT);
		cltMB(V_MAX, TURN_BACK);
	}
}

/* trava o alvo caso saia da mira, esquerda para a direita  */
void travarR(void){
	u16 value = sRight();
	if((value > R_MIN) &&  (value < R_MID)){
		cltMA(V_MIN, TURN_BACK);
		cltMB(V_MIN, TURN_FRONT);
	} else if(value > R_MID){
		cltMA(V_MAX, TURN_BACK);
		cltMB(V_MAX, TURN_FRONT);
	}
}


void dbg(void){
	static u64 time = millis() + TIME_S_READ;
	if(time < millis()){
		printDbgU32("sLeft", sLeft());
		printDbgU32("sRight", sRight());
		printDbgU32("sFront", sFront());
		printDbgU32("sBehind", sBehind());
		printDbgU32("sbRight", sbRight());
		printDbgU32("sbLeft", sbLeft());
		printDbgSTR(NULL, "---------\n");

		time = millis() + TIME_S_READ;
	}
}


//void findL(void){}			// primeira procura a esquerda
//void findR(void){}			// primeira procura a direita
//bool testLine(void){}			// vefificador de linha
//bool testLineR(void){}		// Lina direita
//bool testLineL(void){}		// Linha esquerda




