#ifndef TABLO_H
#define TABLO_H

#include "common.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "stdint.h"

#define DI_Pin GPIO_PIN_3
#define CLK_Pin GPIO_PIN_5
#define LAT_Pin GPIO_PIN_1
#define EN_Pin GPIO_PIN_0


class TABLO
{
    public:
        TABLO();
        void Clear();
        void SendDigit(uint8_t digit);
        void Enable(BOOL en);
        void SendCMD(uint8_t Bright, uint8_t sv, uint8_t nu, uint8_t slp);
//        virtual ~TABLO();

    protected:

    private:
};

extern TABLO tablo;
#endif // TABLO_H
