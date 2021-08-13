#include "clock.h"
#include "tablo.h"
#include "tim0.h"

// Clock clock;

Clock::Clock()
{
    TimeOut = 0;

    mSecond = 0;
    second = 0;
    //ctor
}

//Clock::~Clock()
//{
//    //dtor
//}


void Clock::Tick()
{
    if(tim0.get_timeout(&mSecond) == TRUE)
    {
        tim0.set_timeout(&mSecond,1000);
        second++;
    }

}


void Clock::Time()
{
//    tablo.SendDigit(1);
    if(tim0.get_timeout(&TimeOut) == TRUE)
    {
        tim0.set_timeout(&TimeOut,500);
    }
}

BOOL Clock::getTimeOut()
{
//    TIM0 tim;
    return tim0.get_timeout(&TimeOut);
}

