#include "tim0.h"
#include "common.h"

#include "clock.h"


#include <avr/io.h>
#include <avr/interrupt.h>


uint8_t ReloadValue0 = 1;  // значение перезагрузки таймера 0
uint32_t sysTick = 0;
TIM0 tim0(1000, 64);

void TIM0_Init(uint16_t usFreq, uint16_t usPrescale, uint8_t *ucReload)        //частота; частоты синхронизации таймера 0  ;значение перезагрузки таймера 0
{
	uint8_t ucTemp;
	TCCR0A = 0x00; //stop
	TCCR0B = 0x00; //stop
	ucTemp  = (uint8_t)((F_CPU / (long)usFreq) / (long)usPrescale);
	OCR0A  = ucTemp;
	OCR0B  = ucTemp;
	TCNT0 = 256 - ucTemp;
	*ucReload = TCNT0;
	switch(usPrescale)
	{
		case 1:
			TCCR0B = 0x01;
			break;
		case 8:
			TCCR0B = 0x02;
			break;
		case 64:
			TCCR0B = 0x03;
			break;
		case 256:
			TCCR0B = 0x04;
			break;
		case 1024:
			TCCR0B = 0x05;
			break;
	}
	TIMSK0 |= (1<<TOIE0); //timer interrupt sources

}

TIM0::TIM0()
{

}

TIM0::TIM0(uint16_t usFreq, uint16_t usPrescale)
{
	uint8_t ucTemp;
	TCCR0A = 0x00; //stop
	TCCR0B = 0x00; //stop
	ucTemp  = (uint8_t)((F_CPU / (long)usFreq) / (long)usPrescale);
	OCR0A  = ucTemp;
	OCR0B  = ucTemp;
	TCNT0 = 256 - ucTemp;
	ReloadValue0 = TCNT0;
	switch(usPrescale)
	{
		case 1:
			TCCR0B = 0x01;
			break;
		case 8:
			TCCR0B = 0x02;
			break;
		case 64:
			TCCR0B = 0x03;
			break;
		case 256:
			TCCR0B = 0x04;
			break;
		case 1024:
			TCCR0B = 0x05;
			break;
	}
	TIMSK0 |= (1<<TOIE0); //timer interrupt sources
}

//TIM0::~TIM0()
//{
//    //dtor
//}

void TIM0::set_timeout(uint32_t *TimeOut, uint32_t Value)
{
	*TimeOut = Value + sysTick;
}

BOOL TIM0::get_timeout(uint32_t *TimeOut)
{
	if( *TimeOut < sysTick)
	{
		return TRUE;
	}
	else {
		return FALSE;
	}
}



 ISR(TIMER0_OVF_vect)
{
	TCNT0 = ReloadValue0; //reload counter value
	sysTick++;
}
