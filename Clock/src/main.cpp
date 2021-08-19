/*
 */

#include <avr/io.h>
#include <util/atomic.h>

#include "common.h"
#include "gpio.h"
#include "tim0.h"
#include "tablo.h"
#include "clock.h"
#include "uart.h"

extern GPIO GPIOB;
extern UART uart0;


int main(void)
{
    cli();

    sei();

    while(1)
    {
        uart0.sendByte();

    };

    return 0;
}
