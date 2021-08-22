#ifndef CLOCKT_H
#define CLOCKT_H
#include "stdint.h"

class ClockT
{
    public:
        ClockT();
        void TG();


    protected:

    private:
        uint32_t milis;
        uint8_t sec;
        uint8_t minu;
        uint8_t hour;
        uint8_t dow;
        uint8_t day;
        uint8_t mouth;
        uint8_t ear;

};

#endif // CLOCKT_H
