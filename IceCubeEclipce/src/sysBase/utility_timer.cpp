//#include <dtmf.h>

#include <../include/sysBase/utility_timer.h>
#include <core/pogPWM.h>
#include <core/core.h>
#include <misc.h>
#include <stdint-gcc.h>
#include <stm32f10x.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_rtc.h>
#include <stm32f10x_tim.h>
#include <system_stm32f10x.h>





/* Private macro -------------------------------------------------------------*/
// Timer utilizado (RCC_APB  >> 1 <<  PeriphClockCmd)
#define TIM_DELAY	TIM4
#define PERIPH_DELAY	RCC_APB1Periph_TIM4
#define IQR_CH_DELAY	TIM4_IRQn

/* Private variabel -------------------------------------------------------------*/
volatile uint64_t		ticksCont = 0;						// variable to timed counter


/* Private Functions ------------------------------------------------------------*/
//void init_timer		(uint16_t ms	);					// init timer for delay utility
//void disable_timer_delay	(void		);					// disable timer delay utulity



/**************************************************************************
 * trava o programa até que o tempo extoure
 * ms: minimo 5 ms
 * ms: maximo 65535 ms
**************************************************************************/
void delay_ms(uint64_t ms){								// delay stop system
	ms += millis();
	while(ms > millis());
}

/**************************************************************************
 * retonta façso enquanto o tempo ms não extourar
 * ms: minimo 5 ms
 * ms: maximo 65535 ms
**************************************************************************/
bool timeout_ms(uint64_t ms){								// delay flag
	if(ms > millis()) return false;
	return true;
}


/**************************************************************************
 * utilizada nesse programa para aquisisão do tom dtmf
 *
 *
 *
 * contagem pedriodica precisa
 * divCLK: DIV_CLK_2us5, DIV_CLK_5us, DIV_CLK_10us ,DIV_CLK_100us,
 * DIV_CLK_1ms, DIV_CLK_10ms, DIV_CLK_100ms, DIV_CLK_200ms.
 * le posivel setar outro valores alem dos predefinidos, divCLK é um
 * prescalar da frequancia "SystemCoreClock"
**************************************************************************/
void set_counter(uint32_t divClk){
	SysTick_Config (SystemCoreClock / divClk);
}

/**************************************************************************
 * define de onde contagem de  millis() deve procegir
**************************************************************************/
void write_counter(uint64_t cont){
	ticksCont = cont;
}

/**************************************************************************
 * retorna o contador em ms que, essa contagam é gerada por uma
 * interrupcoa de 1kHz, para isso init_timer_interrupt(1) deve ser
 * definido 1 correspondo ao intervalo de interruocao de 1ms
 * não utilizar como relogio apnas delay, não tem uita precisaoS
**************************************************************************/
uint64_t millis(void){
	return ticksCont;
}

/**************************************************************************
 * gera uma contagam de 0 a 4294967295, sendo uma por segundo
 * caos de contar por mais de um seculo
 * para predefinir o valor da contagam utilise:
 * 	 void RTC_SetCounter(uint32_t CounterValue);
 * para capturar o valor da contagem utilise:
 * 	uint32_t  RTC_GetCounter(void);
**************************************************************************/
void init_rtc(void){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	// CK_RTC clock selection
	PWR_BackupAccessCmd(ENABLE);							// Allow access to BKP Domain
	BKP_DeInit();									// Reset Backup Domain
	RCC_LSICmd(ENABLE);								// Eisable the LSI OSC
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);						// Select the RTC Clock Source
	RTC_SetPrescaler(36363);							// Set 20000 prescaler - for one second interupt
	RCC_RTCCLKCmd(ENABLE);								// Enable the RTC Clock
	RTC_WaitForSynchro();								// Wait for RTC registers synchronization
	RTC_WaitForLastTask();								// Wait until last write operation on RTC registers has finished
}

void init_timer_interrupt(uint16_t ms){
	TIM_TimeBaseInitTypeDef	timerInitStructure;
	NVIC_InitTypeDef nvicStructure;
	RCC_APB1PeriphClockCmd(PERIPH_DELAY, ENABLE);

	TIM_DeInit(TIM_DELAY);				// valores padrores
	timerInitStructure.TIM_Prescaler		= 65535;
	timerInitStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	timerInitStructure.TIM_Period			= ms;	// contagem imperfeita abaixo de 5ms
	timerInitStructure.TIM_ClockDivision		= TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter	= 0;


	nvicStructure.NVIC_IRQChannel = IQR_CH_DELAY;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;


	TIM_TimeBaseInit(TIM_DELAY, &timerInitStructure);
	NVIC_Init(&nvicStructure);
	TIM_ITConfig(TIM_DELAY, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM_DELAY, ENABLE);
	TIM_ClearITPendingBit(TIM_DELAY, TIM_IT_Update);
}


//			<========== private functions ==========>			//



/**************************************************************************
 * desativar o timer para as funções delay_ms e timeout_ms
**************************************************************************/
void disable_timer_delay(void){
	TIM_ClearITPendingBit(TIM_DELAY, TIM_IT_Update);
	TIM_Cmd(TIM_DELAY, DISABLE);
	RCC_APB1PeriphClockCmd(PERIPH_DELAY, DISABLE);
}

/**************************************************************************
 * contador de "set_counter(uint32_t divClk)"
**************************************************************************/
extern "C" {
	void SysTick_Handler(void) {
		__NOP();
	}

	void TIM4_IRQHandler(void) {
		ticksCont++;
		testLineR();		// Lina direita
		testLineL();		// Lina direita
		testLineU();
		TIM_ClearITPendingBit(TIM_DELAY, TIM_IT_Update);
	}
}







