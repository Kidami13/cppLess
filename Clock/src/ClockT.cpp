#include "ClockT.h"
#include "tablo.h"
#include "tim0.h"
ClockT::ClockT()
{
    minu = 13;
    hour = 17;
    day = 22;
    dow = 7;
    mouth = 8;
    ear = 21;
}
void ClockT::TG(){
    if(tim0.get_timeout(&milis)== TRUE){
         tim0.set_timeout(&milis, 1000);

       sec++;
       if(sec > 59){
        minu ++;
        sec = 0;
            if(minu > 59){
            hour++;
            minu = 0;
                if(hour > 23){
                    hour = 0;
                    dow++;
                    if(dow >7){
                        dow = 1;
                    }
                    day++;
                    if(day>30) {
                        mouth++;
                        if(mouth>12){
                            ear++;
                        }


                    }

            }
        }
       }
       tablo.send(hour,minu,sec);
    }
}

