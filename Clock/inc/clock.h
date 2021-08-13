#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "common.h"

class Clock
{
    public:
        Clock();
        void Tick();
        void Time();
        BOOL getTimeOut();
//        virtual ~Clock();

    protected:

    private:
        uint32_t TimeOut;
        uint32_t mSecond;
        uint8_t second;
};


extern Clock clock;

#endif // CLOCK_H
