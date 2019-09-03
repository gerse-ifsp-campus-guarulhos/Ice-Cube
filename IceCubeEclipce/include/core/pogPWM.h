/*
 * pogPWM.h
 *
 *  Created on: 21 de ago de 2019
 *      Author: pibs
 */

#ifndef POGPWM_H_
#define POGPWM_H_


#include <core/protocol.h>
#include <sysBase/utility_timer.h>


#define PERIOD		DIV_CLK_5us
#define DCLY_T		500

void initPWM(void);
void pwmLoop(void);
void pwmENABLE(bool enable);
void setDclyMA(u16 dcly);
void setDclyMB(u16 dcly);

#endif /* POGPWM_H_ */
