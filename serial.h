/* 
 * File:   serial.h
 * Author: Raphael
 *
 * Created on 12 de Abril de 2016, 14:22
 */

#ifndef SERIAL_H
#define	SERIAL_H

#define DataRdy1USART() (PIR1bits.RCIF)
#define putcUSART writeserial()
#define getcUSART readserial()


union USART
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for usart
    unsigned OVERRUN_ERROR:1;   // Overrun Error for usart
    unsigned fill:4;
  };
}USART_Status;

//USART USART_Status;

void putrsserial(const char *data);
void putsserial(char *data);
void writeserial(char data);
char readserial(void);
char Busy1USART(void);
char getc(void);
char DataRdyUSART(void);
void getsUSART(char *buffer, unsigned char len);


//#define WriteToTX2(x)   do{while(TXSTAbits.TRMT); WriteUSART(x);}while(0) /* This prevents from overwriting previous byte */

#endif	/* SERIAL_H */

