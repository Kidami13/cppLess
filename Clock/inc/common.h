#ifndef COMMON_H
#define COMMON_H

#include "stdint.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __INFLASH
        #define __INFLASH PROGMEM const
#endif


typedef enum {
	FALSE=0,
	TRUE=1
} BOOL;



#ifdef __cplusplus
}
#endif

#endif // COMMON_H
