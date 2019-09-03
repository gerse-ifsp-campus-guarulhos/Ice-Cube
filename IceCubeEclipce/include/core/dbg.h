/*
 * dbg.h
 *
 *  Created on: 21 de ago de 2019
 *      Author: pibs
 */

#ifndef DBG_H_
#define DBG_H_

#include <core/protocol.h>

void printDbgSTR(CCHR *label, CCHR *str);
void printDbgU32(CCHR *label, const u32 value);
void serialPrint(CCHR *str, CCHR endChrar);



#endif /* DBG_H_ */
