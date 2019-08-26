/*
 * iwdg.h
 *
 *  Created on: 13 de set de 2018
 *      Author: pibs
 */

#ifndef IWDG_H_
#define IWDG_H_

#include <stdint.h>
#include <stm32f10x_iwdg.h>

void iwdg_Init(uint16_t topCounter, uint8_t Prescaler);
void iwdg_RstCont(void);


#endif /* IWDG_H_ */
