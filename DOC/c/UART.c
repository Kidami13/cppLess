#define  __AVRGCC__

#include "wd.h"
#include "UART.h"
#include "Timer0.h"
#include "SPI.h"
//#include "def.h"
//#include "hc595.h"
//#include "ds1631.h"
#include "sst25vf040b.h"
#include "delay_m.h"
#include "TWI_driver.h"
#include "BUZLog.h"


#define __USEUART0__
//если используется RS485
//#define __USERS485__


#include "Common.h"
#ifdef __ICCAVR__
    #include <macros.h>
#endif
#ifdef __AVRGCC__
    #include <avr/io.h>
    #include <avr/interrupt.h>

    #include <stdio.h>
    #include <string.h>


#endif

#ifdef __USEUART0__

#ifdef __USERS485__
#define rs485_txden     bit_1(PORTD,3)
#define rs485_rxden     bit_0(PORTD,3)

#define rs485_on      bit_1(DDRD,3)
#define rs485_off     bit_0(DDRD,3)

#endif


#define _UART0_TX_SIZE_BUF 300
#define _UART0_RX_SIZE_BUF 20

//передающий буфер
    uchar ucUart0TxBuf_Buf[_UART0_TX_SIZE_BUF];
    uchar ucUart0TxBuf_Tail;
    uchar ucUart0TxBuf_Head;
    uchar ucUart0TxBuf_Count;
    uint  uiUart0TxBuf_CSum;
//приемный буфер
    uchar ucUart0RxBuf_Buf[_UART0_RX_SIZE_BUF];
    uchar ucUart0RxBuf_Tail;
    uchar ucUart0RxBuf_Head;
    uchar ucUart0RxBuf_Count;
    uint  uiUart0RxBuf_CSum;

ushort usUART0_TimeOut;
ushort usUART0_CMDTimeOut;


// __INFLASH uchar ucANS_buf[] = {'A','N','S','1',0,1,100,30,0,1,0x09,0x6A};
//unsigned char ucWHeater = 0;
//unsigned char testbuf[16] = {'C','O','N','F',0x00,0x4E,0x20,0x00,0xC8,0x01,0x2C};
//unsigned char testbuf[16] = {"Start Test\n\r"};
////const
 __INFLASH uchar ucCMD[] = "CMD0";
 __INFLASH char ucTime[6] = "Time ";
// __INFLASH uchar ucErr_CMD[] = "ERR_CMD";
uchar ucRXErr = 0;


void StartMsg(void)
{
    UART0_SendMsg(PSTR("BUZ Log V16.11.16\r\nR_0°C;52,5\r\nTKR;5°C/Om\r\n"));
}



void UART0_ERR_CMD( uchar ucCode )
{
    DelayTimer0mSec(300);
//    DelayTimer0Sec(1);
//    uchar ucTemp;

   if(ucCode)
    {
        UART0_SendMsg(PSTR("Err CMD "));
        UART0_PutChar( ucCode);
        UART0_SendMsg(PSTR("\r\n"));
    }else
    {
        UART0_SendMsg(PSTR("DONE\r\n"));
    }

//    UART0_FlushRxBuf();
    ucUart0RxBuf_Head = ucUart0RxBuf_Tail;
    ucUart0RxBuf_Count = 0;
}


void fSendTime(void)
{
    uchar ucTemp;
    UART0_SendMsg(ucTime);
    uchar ucTimeBuf[7];

    ReadTWI0Cell(TWI_CLOCK, 0, 1, ucTimeBuf, 7);

    ucTemp = ((ucTimeBuf[4] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[4] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);
    UART0_PutChar('.');
    ucTemp = ((ucTimeBuf[5] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[5] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);
    UART0_PutChar('.');
    ucTemp = ((ucTimeBuf[6] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[6] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);

    UART0_PutChar(' ');
    ucTemp = ((ucTimeBuf[3] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[3] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);
    UART0_PutChar(' ');

    ucTemp = ((ucTimeBuf[2] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[2] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);
    UART0_PutChar(':');
    ucTemp = ((ucTimeBuf[1] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[1] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);
    UART0_PutChar(':');
    ucTemp = ((ucTimeBuf[0] >> 4) & 0x07) + 0x30;
    UART0_PutChar(ucTemp);
    ucTemp = (ucTimeBuf[0] & 0x0F) + 0x30;
    UART0_PutChar(ucTemp);
    UART0_PutChar(' ');


//	i=7;
//	while(i)
//    {
//        i--;
//        ucTemp = ((ucTimeBuf[i] >> 4) & 0x07) + 0x30;
//        UART0_PutChar(ucTemp);
//        ucTemp = (ucTimeBuf[i] & 0x0F) + 0x30;
//        UART0_PutChar(ucTemp);
//        UART0_PutChar(' ');
//    }
//    UART0_PutChar('\r');

}



void UART0_CMD02(void)
{
    uchar i, ucTemp1, ucTemp2;
    uchar ucTimeBuf[7];

    i= 6;
    while(i)

    {
        usUART_TimeOut = 10;
        while(ucUart0RxBuf_Count < 2)
        {
            if( usUART_TimeOut == 0)
            {
                UART0_ERR_CMD('2');
                return;
            }
        }
        ucTemp1 = UART0_GetChar();
        ucTemp2 = (ucTemp1 - 0x30) << 4;
        ucTemp1 = UART0_GetChar();

        ucTimeBuf[i] = ucTemp2 + (ucTemp1 - 0x30);
        i--;
    }
    ucTimeBuf[0] = 0;
    WriteTWI0Cell(TWI_CLOCK, 0, 1, ucTimeBuf, 7, 0);

    fSendTime();
    UART0_PutChar('\r');
    UART0_PutChar('\n');

    UART0_ERR_CMD(0);
}


void UART0_CMD(void)
{
    uchar ucRxBuf[5];
    uchar i, ucInCMD;
    if( ucUart0RxBuf_Count > 4)
    {
        for(i = 0;i<4;i++) ucRxBuf[i] = UART0_GetChar();
        if(!mmcmpconst(ucRxBuf,ucCMD,4))
        {
            usUART_TimeOut = 10;
            while(ucUart0RxBuf_Count == 0)
            {
                if( usUART_TimeOut == 0)
                {
                    UART0_ERR_CMD('2');
                    return;
                }
            }
            ucInCMD = UART0_GetChar();
            switch(ucInCMD)
            {
                case 0x31: //Ответ на поиск
                    {
                        StartMsg();
                        fSendTime();
                        fLogPositionRead();
                        break;
                    }
                case 0x32: //установка времени
                    {
                        UART0_CMD02();
                        break;
                    }
                case 0x33: //получение лога
                    {
                        fLogRead();
                        UART0_ERR_CMD(0);
                        break;
                    }
                case 0x39: //clear лога
                    {
                        fLogReset();
                        UART0_ERR_CMD(0);
                        break;
                    }
                default:
                    {
                        UART0_ERR_CMD('1');
                        break;
                    }

            }
        }
        else
        {
            UART0_ERR_CMD('1');
        }
    }
    else
    {
        _WDR_();
    }


}

void UART0_TxBusy(uchar ucCount, uint uiTimeOut)
{
    while(ucUart0TxBuf_Count > (_UART0_TX_SIZE_BUF - ucCount))
    {
        DelayTimer0mSec(uiTimeOut);
    }

}

void UART0_SendVal(ulong ulVal, uchar ucDP)
{
//    uchar ucBuf[6];
//    volatile uchar ucHundred;

    if(ulVal > 100000) UART0_PutChar(0x30 + (ulVal/100000));
//        ucHundred = 0x30 + (ulVal/100000);
    if(ulVal >= 10000) UART0_PutChar(0x30 + (ulVal/10000));
//    else UART0_PutChar(0x30);
    if(ucDP == 4) UART0_PutChar(',');

    if(ulVal >= 1000) UART0_PutChar(0x30 + ((ulVal%10000)/1000));
//    else UART0_PutChar(0x30);
    if(ucDP == 3) UART0_PutChar(',');

    if(ulVal >= 100) UART0_PutChar(0x30 + ((ulVal%1000)/100));
    else UART0_PutChar(0x30);
    if(ucDP == 2) UART0_PutChar(',');

    if(ulVal >= 10) UART0_PutChar(0x30 + ((ulVal%100)/10));
    else UART0_PutChar(0x30);
    if(ucDP == 1) UART0_PutChar(',');

    UART0_PutChar(0x30 + (ulVal%10));


//    ucBuf[0] = 0x30 + ((ulVal%100000)/10000);
//    ucBuf[1] = 0x30 + ((ulVal%10000)/1000);
//    ucBuf[2] = ',';
//    ucBuf[3] = 0x30 + ((ulVal%1000)/100);
//    ucBuf[4] = 0x30 + ((ulVal%100)/10);
//    ucBuf[5] = 0x30 + (ulVal%10);
//
//    UART0_SendStr(ucBuf,6,0);

}

void fIntToBuf(uchar* ucBuf, uint uiVal, uchar ucStartPos, uchar ucLen)
{
    uchar ucTmpBuf[8] = "00000000";
    uchar ucPos;
    if(ucLen > 8) ucLen = 8;
    if(uiVal >= 10000)  ucTmpBuf[3] = (0x30 + (uiVal/10000));
    if(uiVal >= 1000)   ucTmpBuf[4] = (0x30 + ((uiVal%10000)/1000));
    if(uiVal >= 100)    ucTmpBuf[5] = (0x30 + ((uiVal%1000)/100));
    if(uiVal >= 10)     ucTmpBuf[6] = (0x30 + ((uiVal%100)/10));
                        ucTmpBuf[7] = (0x30 + (uiVal%10));
    ucPos = 7;
    while(ucLen)
    {
        ucLen--;
        ucBuf[ucStartPos + ucLen] = ucTmpBuf[ucPos];
        ucPos--;
    }

}

void UART0_SendInt(uint uiVal)
{
    if(uiVal >= 10000) UART0_PutChar(0x30 + (uiVal/10000));
    else UART0_PutChar(0x30);
    if(uiVal >= 1000) UART0_PutChar(0x30 + ((uiVal%10000)/1000));
    else UART0_PutChar(0x30);
    if(uiVal >= 100) UART0_PutChar(0x30 + ((uiVal%1000)/100));
    else UART0_PutChar(0x30);
    if(uiVal >= 10) UART0_PutChar(0x30 + ((uiVal%100)/10));
    else UART0_PutChar(0x30);
    UART0_PutChar(0x30 + (uiVal%10));
}

uchar fHEXToASCII(uchar h)
{
    h &= 0x0F;
    if(h > 9) h += 7;
    h += '0';
    return h;
}


void UART0_SendHEX(uchar ucSym)
{
	uchar ucTempBuf[4] = "xFF";

//	ucTempBuf[1] = pgm_read_byte(&ucHEXBuf[ucSym >> 4]);
//	ucTempBuf[2] = pgm_read_byte(&ucHEXBuf[ucSym & 0x0F]);

	ucTempBuf[1] = fHEXToASCII(ucSym >> 4);
	ucTempBuf[2] = fHEXToASCII(ucSym & 0x0F);
	UART0_SendStr(ucTempBuf, 3, 0);

}





//_______________________Тест CRC_______________________________________________________
unsigned short CS(unsigned char *buf, unsigned char len)
{
	unsigned char i, b16;
	unsigned short usCS = buf[0];

	i = 1;
	while(i < len)
	{
		b16 = 0;
		if(usCS & 0x8000)
		{
			b16 = 1;
		}
		usCS = ((usCS << 1) + buf[i] + b16);
		i++;
	}

	return usCS;
}

//_______________________Init uart_______________________________________________________
void UART0_Init(ulong  ulongFreq)
{
    uint temp_uart;
    UART0_FlushTxBuf();
    UART0_FlushRxBuf();
    #ifdef __USERS485__
    rs485_on;
    #endif


    UCSR0B = 0x00; //disable while setting baud rate
    UCSR0A = 0x00;
    uart0_8bit;

    temp_uart  = (uint)((KVARZ / (ulongFreq*16))-1);
    UBRR0L = (uchar)temp_uart;         //set baud rate lo
    UBRR0H = (uchar)(temp_uart >> 8);  //set baud rate hi

    UCSR0B  = 1<<TXEN0 | 1<<RXEN0 | 1<<TXCIE0| 1<<RXCIE0;
}

//_______________________Передача_______________________________________________________

//возвращает колличество символов передающего буфера
unsigned char UART0_GetTxCount(void)
{
  return ucUart0TxBuf_Count;
}

//"очищает" передающий буфер
void UART0_FlushTxBuf(void)
{
  ucUart0TxBuf_Tail = 0;
  ucUart0TxBuf_Count = 0;
  ucUart0TxBuf_Head = 0;
}

//помещает символ в буфер, инициирует начало передачи
void UART0_PutChar(unsigned char sym)
{
  //если модуль uart свободен и это первый символ
  //пишем его прямо в регистр UDR
  if(((UCSR0A & (1 << UDRE0)) != 0) && (ucUart0TxBuf_Count == 0)) {
    #ifdef __USERS485__
    rs485_txden;
    delay_us(10);
    #endif
    UDR0 = sym;
  } else {
    if (ucUart0TxBuf_Count < _UART0_TX_SIZE_BUF){    //если в буфере еще есть место
        ucUart0TxBuf_Buf[ucUart0TxBuf_Tail] = sym; //помещаем в него символ
        ucUart0TxBuf_Count++;                   //инкрементируем счетчик символов
        ucUart0TxBuf_Tail++;                 //и индекс хвоста буфера
        if (ucUart0TxBuf_Tail == _UART0_TX_SIZE_BUF) ucUart0TxBuf_Tail = 0;
    }
  }
}

//функция посылающая строку по uart`у
void UART0_SendStr(uchar *data, uchar len, uchar PutCS)
{
  uchar sym, sizeof_temp = len;

    if(PutCS){
        uiUart0TxBuf_CSum = CS(data, sizeof_temp);
    }

  while(len){
    sym = *data++;
    len--;
    UART0_PutChar(sym);
  }
    if(PutCS){
        sym = (uchar)(uiUart0TxBuf_CSum >> 8);
        UART0_PutChar(sym);
        sym = (uchar)uiUart0TxBuf_CSum;
        UART0_PutChar(sym);
    }
}

//функция посылающая строку по uart`у
void UART0_SendMsg(const char *data)
{
  uchar sym;
#ifdef __AVRGCC__
  sym = pgm_read_byte(data++);//*data++;
#else
  sym = *data++;
#endif // __AVRGCC__
//  sym = pgm_read_byte(data++);//*data++;
  while(sym)
  {
        UART0_PutChar(sym);
#ifdef __AVRGCC__
        sym = pgm_read_byte(data++);//*data++;
#else
        sym = *data++;
#endif // __AVRGCC__
//        sym = *data++;
  }
}


//обработчик прерывания по завершению передачи
#if defined (__ICCAVR__)
#pragma interrupt_handler uart0_tx_isr:iv_USART0_TXC
void uart0_tx_isr(void)
#elif defined (__AVRGCC__)
ISR(USART_TX_vect)
#endif
{
 //character has been transmitted

  if (ucUart0TxBuf_Count > 0){              //если буфер не пустой
    UDR0 = ucUart0TxBuf_Buf[ucUart0TxBuf_Head]; //записываем в UDR символ из буфера
    ucUart0TxBuf_Count--;                   //уменьшаем счетчик символов
    ucUart0TxBuf_Head++;                 //инкрементируем индекс головы буфера
    if (ucUart0TxBuf_Head == SIZE_BUF) ucUart0TxBuf_Head = 0;
  }else{                                //если буфер пустой
    UART0_FlushTxBuf();                  //обнулить показатели буфера
    #ifdef __USERS485__
    rs485_rxden;                        //переключить на прием rs485
    #endif

  }
  _WDR_();
//  return 0;
}

//_______________________Прием_______________________________________________________
//возвращает колличество символов находящихся в приемном буфере
unsigned char UART0_GetRxCount(void)
{
  return ucUart0RxBuf_Count;
}

unsigned char UART0_RxBufEmpty(void)
{
  uchar ucBufEmp = 1;
  if(ucUart0RxBuf_Head == ucUart0RxBuf_Tail) ucBufEmp = 0;

  return ucBufEmp;
}

//Сохраняет приемный буфер
uchar UART0_SaveRxBuf(uchar* buf, uchar *len)
{
    uchar i = 0;
        if(GetUART_TimeOut() == 0)
        {
            if((UART0_GetRxCount() > 4))
            {
                while(UART0_GetRxCount())
                {
                    buf[i] = UART0_GetChar();
                    i++;
                }
                *len = i;
            };
        }
    return i;

}


//"очищает" приемный буфер
void UART0_FlushRxBuf(void)
{
  unsigned char saveSreg = SREG;
//  cli();
  CLI();                           //disable all interrupts
  ucUart0RxBuf_Tail = 0;
  ucUart0RxBuf_Head = 0;
  ucUart0RxBuf_Count = 0;
  SREG = saveSreg;
}

//чтение буфера
unsigned char UART0_GetChar(void)
{
  unsigned char sym;
  if (ucUart0RxBuf_Count > 0){                     //если приемный буфер не пустой
    sym = ucUart0RxBuf_Buf[ucUart0RxBuf_Head];        //прочитать из него символ
    ucUart0RxBuf_Count--;                          //уменьшить счетчик символов
    ucUart0RxBuf_Head++;                        //инкрементировать индекс головы буфера
    if (ucUart0RxBuf_Head == _UART0_RX_SIZE_BUF) ucUart0RxBuf_Head = 0;
    return sym;                         //вернуть прочитанный символ
  }
  return 0;
}

//const uchar ucHello[] = {'H','E','L','O',0x04,0x3B};
//const uchar ucHello[] = {'C','M','D','1',1,0x08,0x0B};
//const uchar ucHello[] = {'I','n','f','1',0x04,0xFD};
//uchar ucHelloInd = 0;

#ifdef __DEBUG_UART0__
//uchar ucANS_buf[] = {'A','N','S','1',0,1,100,30,0,0,0x09,0x69};
//uchar ucANS_buf[] = "ANS1\x00\x01\x64\x1E\x00\x00\x09\x69";
uchar ucANS_buf[] = "ANS1\x00\x01\x64\x1E\x00\x01\x09\x6A";
#endif // __DEBUG_UART0__


//прерывание по завершению приема
#if defined (__ICCAVR__)
#pragma interrupt_handler uart0_rx_isr:iv_USART0_RXC
void uart0_tx_isr(void)
#elif defined (__AVRGCC__)
ISR(USART_RX_vect)
#endif
{
 //uart has received a character in UDR
//  SetUART_TimeOut(50);
  if (ucUart0RxBuf_Count < SIZE_BUF)
    {                //если в буфере еще есть место
      ucUart0RxBuf_Buf[ucUart0RxBuf_Tail] = UDR0;        //считать символ из UDR в буфер
#ifdef __DEBUG_UART0__
      ucUart0RxBuf_Buf[ucUart0RxBuf_Tail] = ucANS_buf[ucUart0RxBuf_Count];        //считать символ из UDR в буфер

#endif // __DEBUG_UART0__
      ucUart0RxBuf_Tail++;                             //увеличить индекс хвоста приемного буфера
      if (ucUart0RxBuf_Tail == SIZE_BUF) ucUart0RxBuf_Tail = 0;
      ucUart0RxBuf_Count++;                                 //увеличить счетчик принятых символов
    }
}

/**************/

#endif



