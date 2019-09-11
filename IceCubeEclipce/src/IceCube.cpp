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


//#define V_MAX			190	//
//#define V_MID			190	//
//#define V_MIN			190	//


/* Private variables ------------------------------------------------------------------------------------------------------------------------------------ */

bool lineL = false;
bool lineR = false;
bool lineU = false;
bool initOK = false;

/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */
bool empurrar(void);
bool foje(void);
u16 travarL(void);
u16 travarR(void);
u16 travarB(void);
void findR(void);
void findL(void);
void findB(void);
void findF(void);



void dbg(void);

/*******************************************************************************
 * FAZER CONFIG ADICIONAS AQUI
*******************************************************************************/
void setUp(void){
	cltPonteH(true);
	cltMA(TURN_OFF);
	cltMB(TURN_OFF);

	initOK = true;
	printDbgSTR(NULL, "INIT... OK");

	findR();		// primeira procura a direitra, está budado


}


/*******************************************************************************
 * FAZER O PROGRAMA PRINCIPAL AQUI...
*******************************************************************************/
void loop(void){
	if(foje() == false){
		empurrar();
	}
//	dbg();
}


/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* ###################################################################################################################################################### */
/* Private Functions ------------------------------------------------------------------------------------------------------------------------------------ */

/* enpura e resolve mira */
bool empurrar(void){
	u16 value = sFront();
	if(value > R_MID){
		findF();
		return true;
	}

	travarL();
	travarR();
	travarB();
	return false;
}

/* foje da linha */
bool foje(void){
	if(lineL || lineR || lineU){
		if(lineU){
			findF();
		} else {
			findB();
		}

		delay_ms(600);

		findR();

		lineR = false;
		lineL = false;
		lineU = false;
		return true;
	}
	return false;
}

/* trava o alvo caso saia da mira, direira para esquerda  */
u16 travarF(void){
	u16 value = sLeft();
	if(value > R_MID){
		findL();
	}
	return value;
}



/* trava o alvo caso saia da mira, direira para esquerda  */
u16 travarL(void){
	u16 value = sLeft();
	if(value > R_MID){
		findL();
	}
	return value;
}

/* trava o alvo caso saia da mira, esquerda para a direita  */
u16 travarR(void){
	u16 value = sRight();
	if(value > R_MID){
		findR();
	}
	return value;
}

/* trava o alvo caso saia da mira, esquerda para a direita  */
u16 travarB(void){
	u16 value = sBehind();
	if((value > R_MID)){
		findR();
	}
	return value;
}

void testLineR(void){		// Lina direita
	if(initOK == false) return;
	if(sbRight() < 333) lineR = true;
}

void testLineL(void){		// Lina direita
	if(initOK == false) return;
	if(sbLeft() < 333) lineL = true;
}

/* Lina direita */
void testLineU(void){
	if(initOK == false) return;
	if(sbUnder() > 1500) lineU = true;
}

/* primeira procura a direita */
void findR(void){
	cltMA(TURN_FRONT);
	cltMB(TURN_BACK);
}


/* primeira procura a direita */
void findL(void){
	cltMA(TURN_BACK);
	cltMB(TURN_FRONT);
}

/* primeira procura frente */
void findB(void){
	cltMA(TURN_BACK);
	cltMB(TURN_BACK);
}

/* primeira procura traz */
void findF(void){
	cltMA(TURN_FRONT);
	cltMB(TURN_FRONT);
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
		printDbgU32("sbUnder", sbUnder());
		printDbgSTR(NULL, "---------\n");

		time = millis() + TIME_S_READ;
	}
}
