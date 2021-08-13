#include "gpio.h"

void GPIO::WritePin(uint8_t GPIO_Pin, GPIO_PinState PinState)
{
    if(PinState == GPIO_PIN_SET)
        *PORTR |= GPIO_Pin;
    else
        *PORTR &= ~GPIO_Pin;
}

void GPIO::Direct(uint8_t GPIO_Pin, GPIO_PinDirect PinDir)
{
    if(PinDir == GPIO_PIN_OUT)
        *DDRR |= GPIO_Pin;
    else
        *DDRR &= ~GPIO_Pin;
}
