#include "uart.h"

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

#define _UART0_TX_SIZE_BUF 300
#define _UART0_RX_SIZE_BUF 20

//#define uart0_5bit     UCSR0C = (UCSR0C & 0xF9) | 0
//#define uart0_6bit     UCSR0C = (UCSR0C & 0xF9) | 0x02
//#define uart0_7bit     UCSR0C = (UCSR0C & 0xF9) | 0x04
//#define uart0_8bit     UCSR0C = (UCSR0C & 0xF9) | 0x06
//

char Uart0TxBuf[_UART0_TX_SIZE_BUF];

char Uart0RxBuf[_UART0_RX_SIZE_BUF] = "test\r\ntOst\r\n0123456";
uint16_t RxXferCount = 15;


// __INFLASH char strCMD[] = "CMD0";

UART uart0(9600);

UART::UART()
{
    //ctor
}

UART::UART(uint32_t  u32Freq)
{
    TimeOut = 0;
    pTxBuffPtr = 0;
    TxXferSize = 0;

    pRxBuffPtr = Uart0RxBuf;
    RxXferSize = _UART0_RX_SIZE_BUF;
//    RxXferCount = 5;
    RxXferLen = 0;
    RxXferTail = 0;
    RxXferHead = 0;
    pRxBuffPtr[19] = '7';



//    uint32_t temp_uart;
//    UART0_FlushTxBuf();
//    UART0_FlushRxBuf();
//    #ifdef __USERS485__
//    rs485_on;
//    #endif


//    UCSR0B = 0x00; //disable while setting baud rate
//    UCSR0A = 0x00;
//    UCSR0C = 1<<UCSZ01 | 1<<UCSZ00; //uart0_8bit
//    UBRR0 = (uint16_t)((F_CPU / (u32Freq*16))-1);
//     //Transmitter Enable, Receiver Enable, TX Complete Interrupt Enable, RX Complete Interrupt Enable
//    UCSR0B  = 1<<TXEN0 | 1<<RXEN0 | 0<<TXCIE0| 1<<RXCIE0|0<<UCSZ02;
}
/***************************************************************************************/
/**               Transmitter                                                         **/
/***************************************************************************************/

//ISR(USART_TX_vect)
//{
// //character has been transmitted
//
////  if (ucUart0TxBuf_Count > 0){              //если буфер не пустой
////    UDR0 = ucUart0TxBuf_Buf[ucUart0TxBuf_Head]; //записываем в UDR символ из буфера
////    ucUart0TxBuf_Count--;                   //уменьшаем счетчик символов
////    ucUart0TxBuf_Head++;                 //инкрементируем индекс головы буфера
////    if (ucUart0TxBuf_Head == SIZE_BUF) ucUart0TxBuf_Head = 0;
////  }else{                                //если буфер пустой
////    UART0_FlushTxBuf();                  //обнулить показатели буфера
////    #ifdef __USERS485__
////    rs485_rxden;                        //переключить на прием rs485
////    #endif
////  }
////  _WDR_();
//}

//    char str[] = "0test";
//    uart0.WriteLine(str);
//
//    uart0.WriteLine(PSTR("1test"));


void UART::WriteLine(char *pData)
{
//    char tmpBuf[100];
//    strcpy_P(buffer2,header_1);

    while(TxXferCount > 0) Transmit();

    pTxBuffPtr = (uint8_t*)Uart0TxBuf;
//    TxXferCount = sprintf_P((char*)pTxBuffPtr, PSTR( "%s\r\n"), pData);
    TxXferCount = sprintf((char*)pTxBuffPtr, "%s\r\n", pData);
}

void UART::WriteLine(const char *pData)
{
//    char tmpBuf[100];
//    strcpy_P(buffer2,header_1);

    while(TxXferCount > 0) Transmit();

    pTxBuffPtr = (uint8_t*)Uart0TxBuf;
//    TxXferCount = sprintf_P((char*)pTxBuffPtr, PSTR( "%S\r\n"), pData);
    TxXferCount = sprintf((char*)pTxBuffPtr, "%s\r\n", pData);
}

void UART::Write(uint8_t *pData, uint16_t Size)
{
    while(TxXferCount > 0) Transmit();

    pTxBuffPtr = pData;
    TxXferCount = Size;

}

void UART::Transmit()
{
    RxXferCount = 7;
    Uart0RxBuf[5] = '\n';
    if(TxXferCount > 0)
    {
//        uint8_t UDRE0_bit = 1 << UDRE0;
//        if((UCSR0A & UDRE0_bit) == UDRE0_bit)
//        {
//            UDR0 = *pTxBuffPtr;
//            pTxBuffPtr++;
//            TxXferCount--;
//        }
    }
}

/***************************************************************************************/
/**               Receiver                                                            **/
/***************************************************************************************/
BOOL UART::findendl()
{
    BOOL ret = FALSE;
    while(ret == FALSE)
    {
        if(RxXferTail == RxXferCount) break;
        RxXferLen++;
        if(pRxBuffPtr[RxXferTail] == '\n')
        {
            pRxBuffPtr[RxXferTail] = 0;
            ret = TRUE;
        }
        RxXferTail++;
        if(RxXferTail > _UART0_RX_SIZE_BUF - 1) RxXferTail = 0;
    }
    return ret;
}
char UART::getChar()
{
    char c;
    c = pRxBuffPtr[RxXferHead];
    RxXferHead++;
    if(RxXferHead > _UART0_RX_SIZE_BUF - 1) RxXferHead = 0;
    return c;
}

uint16_t UART::ReadLine(char *pData)
{
    uint16_t len = 0;
    *pData = 0;
    if(findendl())
    {
        len = RxXferLen;
        for(uint8_t i = 0; i < RxXferLen; i++)
            pData[i] = getChar();
        RxXferLen = 0;
    }
    return len;
}

//ISR(USART_RX_vect)
//{
//    Uart0RxBuf[RxXferCount++] = UDR0;
//    if(RxXferCount > _UART0_RX_SIZE_BUF - 1) RxXferCount = 0;
//}
