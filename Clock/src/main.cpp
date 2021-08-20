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

    uint8_t test[] = "Test\r\ntest       ";
    uart0.Transmit(test,10);

    while(1)
    {
        uart0.sendByte();

    };

    return 0;
}
