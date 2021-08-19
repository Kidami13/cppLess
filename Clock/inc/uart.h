#ifndef UART_H
#define UART_H

#include "stdint.h"

#include "common.h"


class UART
{
    public:
        UART();
        UART(uint32_t  u32Freq);
        void Transmit(uint8_t *pData, uint16_t Size);
        void sendByte();

    protected:

    private:
        uint8_t gState;

        uint8_t *pTxBuffPtr;
        uint16_t TxXferSize;
        uint16_t TxXferCount;

        uint32_t TimeOut;

};

#endif // UART_H
