#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include "stdint.h"

typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;

typedef enum
{
  GPIO_PIN_IN = 0,
  GPIO_PIN_OUT
} GPIO_PinDirect;

#define GPIO_PIN_0                 ((uint8_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint8_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint8_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint8_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint8_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint8_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint8_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint8_t)0x0080)  /* Pin 7 selected    */

typedef struct
{
    volatile uint8_t *PINR;
    volatile uint8_t *DDRR;
    volatile uint8_t *PORTR;
} GPIO_TypeDef;



class GPIO
{
    public:
        GPIO(volatile uint8_t * pin, volatile uint8_t * ddr, volatile uint8_t * port)
        {
            PINR = pin;
            DDRR = ddr;
            PORTR = port;
        }

        void WritePin(uint8_t GPIO_Pin, GPIO_PinState PinState);
        void Direct(uint8_t GPIO_Pin, GPIO_PinDirect PinDir);

    protected:

    private:
            volatile uint8_t *PINR;
            volatile uint8_t *DDRR;
            volatile uint8_t *PORTR;
};



#endif // GPIO_H
