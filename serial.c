/* 
 * File:   serial.h
 * Author: Raphael
 *
 * Created on 12 de Abril de 2016, 14:22
 */

#include "serial.h"
#include "config.h"

void putrsserial(const char const *data)
{
  do
  {  // Transmit a byte
    while(!TXSTAbits.TRMT);
    writeserial(*data);
  } while( *data++ );
}

void putsserial(char *data)
{
  do
  {  // Transmit a byte
    while(!TXSTAbits.TRMT);
    writeserial(*data);
  } while( *data++ );
}

void writeserial(char data)
{
  TXREG = data;      // Write the data byte to the USART
}

char readserial(void)		//this function can be removed by macro #define ReadUSART RCREG
{
  char data;   // Holds received data

  USART_Status.val &= 0xf2;          // Clear previous status flags

  if(RCSTAbits.RX9)                  // If 9-bit mode
  {
    USART_Status.RX_NINE = 0;        // Clear the receive bit 9 for USART
    if(RCSTAbits.RX9D)               // according to the RX9D bit
      USART_Status.RX_NINE = 1;
  }

  if(RCSTAbits.FERR)                 // If a framing error occured
    USART_Status.FRAME_ERROR = 1;    // Set the status bit

  if(RCSTAbits.OERR)                 // If an overrun error occured
    USART_Status.OVERRUN_ERROR = 1;  // Set the status bit

  data = RCREG;                      // Read data

  return (data);                     // Return the received data
}

char Busy1USART(void)
{
  if(!TXSTA1bits.TRMT) // Is the transmit shift register empty
    return 1;          // No, return FALSE
    return 0;            // Return TRUE
}

char DataRdyUSART(void)
{
  if(PIR1bits.RCIF)  // If RCIF is set
    return 1;  // Data is available, return TRUE
  return 0;  // Data not available, return FALSE
}


void getsUSART(char *buffer, unsigned char len)
{
  char i;    // Length counter
  unsigned char data;

  for(i=0;i<len;i++)  // Only retrieve len characters
  {
    while(!DataRdyUSART());// Wait for data to be received

    data = readserial();    // Get a character from the USART
                           // and save in the string
    *buffer = data;
    buffer++;              // Increment the string pointer
  }
}


