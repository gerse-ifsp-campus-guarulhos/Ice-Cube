/*
 * core.h
 *
 *  Created on: 14 de ago de 2019
 *      Author: pibs
 */

#include <core/protocol.h>

void startUpHardware(void);			//
void coreMonitor(void);				//
void blkLed(void);				// pisca led 13


u16 sLeft(void);				// sharp
u16 sRight(void);				// sharp
u16 sFront(void);				// sharp
u16 sBehind(void);				// sharp
u16 sbRight(void);				// TCRT500
u16 sbLeft(void);				// TCRT500
u16 sbUnder(void);				// ldr

void cltPonteH(bool stop);			//
void cltMA(u8 turn);				//
void cltMB(u8 turn);				//

void loop(void);				//
void setUp(void);				//


void testLineR(void);		// Lina direita
void testLineL(void);		// Lina resquerda
void testLineU(void);		// Lina tras
