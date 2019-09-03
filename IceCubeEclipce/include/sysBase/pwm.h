/*
 * pwm.h
 *
 *  Created on: 13 de set de 2018
 *      Author: pibs
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

// minamonicos
#define PWM_DISABLE_ALL	0
#define PWM_CH1		1
#define PWM_CH2		2
#define PWM_CH3		3
#define PWM_CH4		4




// controle do timer do pwm
void pwm_SetFreg(uint32_t prescaler, uint16_t period);
void pwm_DisableALL(void);

// controle do canal do time como função out pwm
void pwm_OnOffCh(uint8_t ch, uint8_t state);
void pwm_SetCh(uint8_t ch, uint16_t dutyCycleON);


#endif /* PWM_H_ */
