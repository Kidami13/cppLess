/*
 */

#include <avr/io.h>
#include <util/atomic.h>
#include "ClockT.h"
#include "common.h"
#include "gpio.h"
#include "tim0.h"
#include "tablo.h"

#include "uart.h"

extern GPIO GPIOB;
extern UART uart0;
ClockT clock;


int main(void)
{
    cli();

    sei();

    uint8_t test[] = "Test\r\ntest       ";
    uart0.Transmit(test,10);

    while(1)
    {
        uart0.sendByte();
        clock.TG();


    };

    return 0;
}
