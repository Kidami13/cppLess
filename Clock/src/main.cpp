/*
 */

#include <avr/io.h>
#include <util/atomic.h>

#include "common.h"
#include "gpio.h"
#include "tim0.h"
#include "tablo.h"
#include "clock.h"

extern GPIO GPIOB;


int main(void)
{
    cli();
//    TIM0_Init(1000,64,&ReloadValue0);

    sei();



    // Insert code

    while(1)
    {
//        clock.Tick();
//        clock.Time();

    }
    ;

    return 0;
}
