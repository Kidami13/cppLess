#ifndef __UART_H__
#define __UART_H__

#include "Common.h"


#define __USEUART0__
//если используется RS485
//#define __USERS485__


#define UART_RX_EMP  0
#define UART_RX_OK   1
#define UART_ERR_CRC 2


//размер буфера
#define SIZE_BUF 200

#define uart0_asynh     bit_0(UCSRC,UMSEL)
#define uart0_synh      bit_1(UCSRC,UMSEL)

#define uart0_5bit     UCSR0C = (UCSR0C & 0xF9) | 0
#define uart0_6bit     UCSR0C = (UCSR0C & 0xF9) | 0x02
#define uart0_7bit     UCSR0C = (UCSR0C & 0xF9) | 0x04
#define uart0_8bit     UCSR0C = (UCSR0C & 0xF9) | 0x06

#define uart0_rx_on      bit_1(UCSR0B,RXEN)
#define uart0_rx_off     bit_0(UCSR0B,RXEN)

#define uart0_rx_comp_intr_on      bit_1(UCSR0B,RXCIE)
#define uart0_rx_comp_intr_off     bit_0(UCSR0B,RXCIE)

#define uart0_tx_on      bit_1(UCSR0B,TXEN0)
#define uart0_tx_off     bit_0(UCSR0B,TXEN0)

#define uart0_tx_comp_intr_on      bit_1(UCSR0B,TXCIE0)
#define uart0_tx_comp_intr_off     bit_0(UCSR0B,TXCIE0)

extern ushort usUART0_CMDTimeOut;

extern __INFLASH char ucTime[6];
unsigned short CS(unsigned char *buf, unsigned char len);

void UART0_Init(ulong);

void UART0_ERR_CMD( uchar ucCode );
unsigned char UART0_GetTxCount(void); //взять число символов передающего буфера
void UART0_FlushTxBuf(void); //очистить передающий буфер
void UART0_PutChar(unsigned char sym); //положить символ в буфер
void UART0_SendStr(uchar *data, uchar len, uchar PutCS); //послать строку по usart`у
void UART0_SendMsg(const char *data); //послать сообщеие

void StartMsg(void);
void UART0_SendHEX(uchar ucSym); //Преобразовать в строку типа HEX и передать
void UART0_SendInt(uint uiVal);
void UART0_SendVal(ulong ulVal, uchar ucDP);
void fIntToBuf(uchar* ucBuf, uint uiVal, uchar ucStartPos, uchar ucLen);
void fSendTime(void);
void UART0_TxBusy(uchar ucCount, uint uiTimeOut);

unsigned char UART0_GetRxCount(void); //взять число символов в приемном буфере
unsigned char UART0_RxBufEmpty(void); //буфер пустой голова равна хвосту
void UART0_FlushRxBuf(void); //очистить приемный буфер
unsigned char UART0_GetChar(void); //прочитать приемный буфер usart`a
uchar UART0_SaveRxBuf(uchar* buf, uchar* len); //Сохранить приемный буфер
void UART0_CMD(void);

//void fSendTime(void);

#endif //__UART_H__
