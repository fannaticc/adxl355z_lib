/* 
 * File:   dog104.c
 * Author: Raphael
 *
 * Created on August 26, 2019, 10:26 AM
 */

#include <stdint.h>
#include <stdbool.h>

#include "dog104.h"
#include "my_i2c.h"

//--- module global varibles ---

unsigned char dh_function_set = 0x38;  //double hight font off default

//--- waitstates for testing ---
//use these macros to slow the communication down
#define WAITST 2
void Wait (uint8_t waitstates)
{
	uint8_t i = 0;
	for (i = 0; i < waitstates; i++)
	{
		asm("NOP");
	}
}

//-----------------------------------------------------
//Func: initDispl()
//Desc: inits Display
//-----------------------------------------------------
void initDispl(void)
{
	uint8_t i;
    
	//Perform a display reset
    /*RES = 0;
    __delay_ms(1);
    RES=1;
    __delay_ms(1);*/
    
	//init Display
	WriteIns(0x3A);	//8-Bit data length extension Bit RE=1; REV=0
	WriteIns(0x09);	//4 line display
#ifdef DISP_BOTTOMVIEW
	WriteIns(BOTTOMVIEW);	//Bottom view
#endif
#ifdef DISP_TOPVIEW
	WriteIns(TOPVIEW);	//Bottom view
#endif
	WriteIns(0x1E);	//Bias setting BS1=1
	WriteIns(0x39);	//8-Bit data length extension Bit RE=0; IS=1
	WriteIns(0x1B);	//BS0=1 -> Bias=1/6
	WriteIns(0x6E); //Devider on and set value
	WriteIns(0x56); //Booster on and set contrast (BB1=C5, DB0=C4)
	WriteIns(0x7A); //Set contrast (DB3-DB0=C3-C0)
	WriteIns(dh_function_set); //8-Bit data length extension Bit RE=0; IS=0

	ClrDisplay();
	DisplayOnOff(DISPLAY_ON | CURSOR_ON | BLINK_ON);
}

//-----------------------------------------------------
//Func: WriteChar(character)
//Desc: sends a single character to display
//-----------------------------------------------------
void WriteChar(int8_t character)
{
	WriteData(character);
}

//-----------------------------------------------------
//Func: WriteString(string)
//Desc: sends a string to display, must be 0 terminated
//-----------------------------------------------------
void WriteString(int8_t *stringi)
{
	do
	{
		WriteData(*stringi++);
	}
	while(*stringi);
}

//-----------------------------------------------------
//Func: WriteString(string)
//Desc: sends a string to display, must be 0 terminated
//-----------------------------------------------------
void DisplLines(uint8_t lines)
{
	if(lines == DOUBLEHEIGHT_OFF)
	{
		WriteIns(lines);
		dh_function_set = 0x38;
	}
	else
	{
		WriteIns(0x3C);
		WriteIns(0x3A);
		WriteIns(lines);
		WriteIns(0x3C);
		dh_function_set = 0x3C;
	}
}

//-----------------------------------------------------
//Func: SetPostion(postion)
//Desc: set postion
//-----------------------------------------------------
void SetPostion(int8_t pos)
{
	WriteIns(LCD_HOME_L1 + pos);
}

//-----------------------------------------------------
//Func: DisplayOnOff(control)
//Desc: use definitions of header file to set display
//-----------------------------------------------------
void DisplayOnOff(int8_t data)
{
	WriteIns(0x08 + data);
}

//-----------------------------------------------------
//Func: DefineCharacter(memory postion, character data)
//Desc: stores an own defined character
//-----------------------------------------------------
void DefineCharacter(uint8_t postion, uint8_t *data)
{
	uint8_t i = 0;
	WriteIns(0x40 + 8 * postion);

	for (i = 0; i < 8; i++)
	{
		WriteData(data[i]);
	}
	SetPostion(LINE1);
}

//-----------------------------------------------------
//Func: ClrDisplay
//Desc: Clears entire Display content and set home pos
//-----------------------------------------------------
void ClrDisplay(void)
{
	WriteIns(0x01);
	SetPostion(LINE1);
}

//-----------------------------------------------------
//Func: SetContrast
//Desc: Sets contrast 0..63
//-----------------------------------------------------
void SetContrast(uint8_t contr)
{
	WriteIns(0x39);
	WriteIns(0x54 | (contr >> 4));
	WriteIns(0x70 | (contr & 0x0F));
	WriteIns(dh_function_set);
}

//-----------------------------------------------------
//Func: SetView
//Desc: view bottom view(0x06), top view (0x05)
//-----------------------------------------------------
void SetView(uint8_t view)
{
	WriteIns(0x3A);
	WriteIns(view);
	WriteIns(dh_function_set);
}

//-----------------------------------------------------
//Func: SetROM
//Desc: Changes the Rom code (ROMA=0x00, ROMB=0x04, ROMC=0x0C)
//---------------------------------------------------
void SetROM(uint8_t value)
{
	WriteIns(0x2A);
	WriteIns(0x72);
	WriteData(value);
	WriteIns(0x28);
}

//-----------------------------------------------------
//Func: WriteIns(instruction)
//Desc: sends instruction to display
//-----------------------------------------------------
void WriteIns(int8_t ins)
{
	while(CheckBusy() == 1);
	IIC_put(0x80, ins); //Controllbyte C0=0 & D/C = 0)
}

//-----------------------------------------------------
//Func: WriteData(data)
//Desc: sends data to display
//-----------------------------------------------------
void WriteData(int8_t data)
{
	while(CheckBusy() == 1);
	IIC_put(0x40, data); //Controllbyte C0=0 & D/C = 1)
}

//-----------------------------------------------------
//Func: CheckBusy()
//Desc: checks if display is idle
//-----------------------------------------------------
uint8_t CheckBusy(void)
{
    return (IIC_get() & 0x80);  //ckeck busyFlag
}

//-----------------------------------------------------
//Func: IIC_put()
//Desc: waits till data is sent over the iic interface
//		returns 1 success, 0 error
//-----------------------------------------------------
void IIC_put(uint8_t cb, uint8_t db)
{
    I2C_Start(); //IIC_Start(); //Start IIC interface
	I2C_WriteByte(DOG104_ADR); //adress
	I2C_WriteByte(cb); //control byte
	I2C_WriteByte(db); //data byte
	I2C_Stop(); //Stop IIC transfer
}

//-----------------------------------------------------
//Func: IIC_get()
//Desc: Gets one byte from display (busy flag check)
//-----------------------------------------------------
static uint8_t IIC_get(void)
{
	uint8_t ret;
    
    I2C_Start();
    I2C_WriteByte(DOG104_ADR);
    I2C_WriteByte(0x80);
    I2C_RepeatedStart();
    I2C_WriteByte(DOG104_ADR + 1);
    ret = I2C_ReadByte(0); // Read the data
    I2C_Stop();
    
	return ret;
}