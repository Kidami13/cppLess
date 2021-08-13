#ifndef TIM0_H
#define TIM0_H

#include "stdint.h"
#include "common.h"

class TIM0
{
    public:
        TIM0();
        TIM0(uint16_t usFreq, uint16_t usPrescale);
//        virtual ~TIM0();

        BOOL get_timeout(uint32_t *TimeOut);
        void set_timeout(uint32_t *TimeOut, uint32_t Value);

    protected:

    private:
};


extern TIM0 tim0;

#endif // TIM0_H
