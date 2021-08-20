#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

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

    char Uart0RxBuf[_UART0_RX_SIZE_BUF];
    char *pRxBuffPtr;
    uint16_t RxXferSize;

    char *pdata8bits;
    uint16_t RxXferCount;

//    char Uart0RxWBuf[_UART0_RX_SIZE_BUF];
//    char *WorkRxBuf;

    uint16_t LastRxIdx;


 __INFLASH char strCMD[] = "CMD0";

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

    LastRxIdx = 0;
//    WorkRxBuf = Uart0RxWBuf;

    pdata8bits = pRxBuffPtr;
    RxXferCount = RxXferSize;

//    uint32_t temp_uart;
//    UART0_FlushTxBuf();
//    UART0_FlushRxBuf();
//    #ifdef __USERS485__
//    rs485_on;
//    #endif


    UCSR0B = 0x00; //disable while setting baud rate
    UCSR0A = 0x00;
    UCSR0C = 1<<UCSZ01 | 1<<UCSZ00; //uart0_8bit
//    UCSR0C = (UCSR0C & 0xF9) | 0x06; //uart0_8bit

    UBRR0 = (uint16_t)((F_CPU / (u32Freq*16))-1);

     //Transmitter Enable, Receiver Enable, TX Complete Interrupt Enable, RX Complete Interrupt Enable
    UCSR0B  = 1<<TXEN0 | 1<<RXEN0 | 0<<TXCIE0| 1<<RXCIE0|0<<UCSZ02;
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

void UART::Transmit(uint8_t *pData, uint16_t Size)
{
    while(TxXferCount > 0) sendByte();

    pTxBuffPtr = pData;
    TxXferCount = Size;

}

void UART::sendByte()
{
    if(TxXferCount > 0)
    {
        uint8_t UDRE0_bit = 1 << UDRE0;
        if((UCSR0A & UDRE0_bit) == UDRE0_bit)
        {
            UDR0 = *pTxBuffPtr;
            pTxBuffPtr++;
            TxXferCount--;
        }
    }
}

/***************************************************************************************/
/**               Receiver                                                            **/
/***************************************************************************************/


ISR(USART_RX_vect)
{
    *pdata8bits = UDR0;
    pdata8bits++;
    RxXferCount++;
    if(RxXferCount > RxXferSize)
    {
        RxXferCount = 0;
        pdata8bits = pRxBuffPtr;
    }
}
