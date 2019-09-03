/*
 * utility_timer.h
 *
 *  Created on: 4 de set de 2018
 *      Author: pibs
 */

#ifndef UTILITY_TIMER_H_
#define UTILITY_TIMER_H_

#include <stdint.h>


#define DIV_CLK_2us5	400000
#define DIV_CLK_5us	200000
#define DIV_CLK_10us	100000
#define DIV_CLK_100us	10000
#define DIV_CLK_1ms	1000
#define DIV_CLK_10ms	100
#define DIV_CLK_100ms	10
#define DIV_CLK_200ms	5

#define DIV_CLK_16KHZ	16000
#define DIV_CLK_8KHZ	8000
#define DIV_CLK_1KHZ	2000
#define DIV_CLK_432HZ	864
#define DIV_CLK_1400HZ	2800
#define DIV_CLK_2300HZ	4600
#define DIV_CLK_100HZ	200

void delay_ms(uint64_t ms);
bool timeout_ms(uint64_t ms);
void set_counter(uint32_t divClk);
void write_counter(uint64_t cont);
uint64_t millis(void);
void init_rtc(void);
void init_timer_interrupt(uint16_t ms);

#endif /* UTILITY_TIMER_H_ */
