#ifndef UART_H
#define UART_H

#include "stdint.h"

#include "common.h"


class UART
{
    public:
        UART();
        UART(uint32_t  u32Freq);
        void Write(uint8_t *pData, uint16_t Size);
        void WriteLine(const char *pData);
        void WriteLine(char *pData);
        void Transmit();

        uint16_t ReadLine(char *pData);

    protected:

    private:
        uint8_t gState;

        uint8_t *pTxBuffPtr;
        uint16_t TxXferSize;
        uint16_t TxXferCount;

        char *pRxBuffPtr;
        uint16_t RxXferSize;
        uint16_t RxXferHead;
        uint16_t RxXferTail;
        uint16_t RxXferLen;

        uint32_t TimeOut;

        BOOL findendl();
        char getChar();
};


extern UART uart0;
#endif // UART_H
