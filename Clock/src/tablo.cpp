#include "tablo.h"

#include "gpio.h"

#include "string.h"
#include "stdio.h"
#include <avr/pgmspace.h>



GPIO GPIOB(&PINB, &DDRB, &PORTB);
//
TABLO tablo;

//__INFLASH
uint8_t s_tabl[]{
//    0         1           2
    0b10000001, 0b00000010, 0b00000100, 0b00000100, 0b00000100, 0b00000100};

void delay_us(uint16_t us)
{
    uint32_t d_count = F_CPU /1000000/us ;
    while(d_count) d_count--;
}

void ClkTick()
{
    GPIOB.WritePin(CLK_Pin,GPIO_PIN_SET);
    delay_us(1);
    GPIOB.WritePin(CLK_Pin,GPIO_PIN_RESET);
    delay_us(1);
}

void LATTick()
{
    GPIOB.WritePin(LAT_Pin,GPIO_PIN_SET);
    delay_us(1);
    GPIOB.WritePin(LAT_Pin,GPIO_PIN_RESET);
    delay_us(1);
}

TABLO::TABLO()
{
    GPIOB.WritePin(DI_Pin|CLK_Pin|LAT_Pin,GPIO_PIN_RESET);
    GPIOB.WritePin(EN_Pin,GPIO_PIN_SET);

    GPIOB.Direct(EN_Pin|DI_Pin|CLK_Pin|LAT_Pin, GPIO_PIN_OUT);

    Enable(TRUE);
    SendCMD(30, 0, 0, 0);
    Clear();
}

void TABLO::SendCMD(uint8_t Bright, uint8_t sv, uint8_t nu, uint8_t slp)
{
    GPIOB.WritePin(LAT_Pin,GPIO_PIN_SET);
    for(uint8_t i = 0; i<4; i++) ClkTick();
    GPIOB.WritePin(LAT_Pin,GPIO_PIN_RESET);

    for(uint8_t d = 0; d < 3; d++)
    {
        for(uint8_t i = 7; i > 0; i--)
        {
            if(((Bright >> (i-1)) & 1) == 1) GPIOB.WritePin(DI_Pin, GPIO_PIN_SET);
            else GPIOB.WritePin(DI_Pin, GPIO_PIN_RESET);
            ClkTick();
        }

        if(sv) GPIOB.WritePin(DI_Pin, GPIO_PIN_SET);
        else GPIOB.WritePin(DI_Pin, GPIO_PIN_RESET);
        ClkTick();

        if(nu) GPIOB.WritePin(DI_Pin, GPIO_PIN_SET);
        else GPIOB.WritePin(DI_Pin, GPIO_PIN_RESET);
        ClkTick();

        if(slp) GPIOB.WritePin(DI_Pin, GPIO_PIN_SET);
        else GPIOB.WritePin(DI_Pin, GPIO_PIN_RESET);
        ClkTick();
    }


    LATTick();

}

void TABLO::SendDigit(uint8_t digit)
{
    uint8_t d = digit;
//    memcpy_P(&d, &s_tabl[digit], 1);

    for(uint8_t i = 0; i < 8; i++)
    {
        if(((d >> i) & 1) == 1) GPIOB.WritePin(DI_Pin, GPIO_PIN_SET);
        else GPIOB.WritePin(DI_Pin, GPIO_PIN_RESET);
        ClkTick();
    }
}

void TABLO::Clear()
{
    for(uint8_t i = 0; i < 6; i++) SendDigit(2);
    LATTick();

}

void TABLO::Enable(BOOL en)
{
    if(en) GPIOB.WritePin(EN_Pin,GPIO_PIN_RESET);
    else GPIOB.WritePin(EN_Pin,GPIO_PIN_SET);
}
