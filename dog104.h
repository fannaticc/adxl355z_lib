/* 
 * File:   dog104.h
 * Author: Raphael
 *
 * Created on August 26, 2019, 10:26 AM
 */

#ifndef DOG104_H
#define	DOG104_H

#include <stdint.h>

//Ported from: https://www.lcd-module.com/support/application-note.html
//             https://www.lcd-module.com/fileadmin/downloads/development%20service/DOGS104_SSD1803A/DOGS104_SSD1803A_IIC.zip
// By: Raphael Pereira


#define DOG104_ADR 0x78   //0x7A if SA0 is set to 1

//--- Standard definitions for LCD ---
#define DISP_BOTTOMVIEW
//#define DISP_TOPVIEW



#ifdef DISP_BOTTOMVIEW
#define LINE1	0
#endif

#ifdef DISP_TOPVIEW
#define LINE1	10
#endif

/*
//HD44780
#define LINE2	LINE1+0x40
#define LINE3	LINE1+0x14
#define	LINE4 	LINE2+0x14
*/
//KS0073/SSD1803(A)
#define LCD_HOME_L1	0x80
#define LINE2	LINE1+0x20
#define LINE3	LINE1+0x40
#define	LINE4 	LINE1+0x60

#define DISPLAY_ON 	0x04
#define DISPLAY_OFF 0x03
#define CURSOR_ON	0x02
#define CURSOR_OFF	0x05
#define BLINK_ON	0x01
#define BLINK_OFF	0x06

#define TOPVIEW		0x05
#define BOTTOMVIEW	0x06

#define ROMA		0x00
#define ROMB		0x04
#define ROMC		0x0C

#define DOUBLEHEIGHT_TOP 	0x1E
#define DOUBLEHEIGHT_MID 	0x16
#define DOUBLEHEIGHT_BOT 	0x12
#define DOUBLEHEIGHT_DOUBLE	0x1A
#define DOUBLEHEIGHT_OFF	0x38

//definitions
#define TIME 2000
#define CONTRSTART 35
#define CONTRSPAN  15

void initDispl(void);
void WriteChar(int8_t character);
void WriteString(int8_t *stringi);
void SetPostion(int8_t pos);
void DisplayOnOff(int8_t data);
void DefineCharacter(uint8_t postion, uint8_t *data);
void ClrDisplay(void);
void SetContrast(uint8_t contr);
void SetView(uint8_t view);
void SetROM(uint8_t value);
void DisplLines(uint8_t lines);

#define GETCURSORADDR()	CheckBusy()

//Normally you don't need these functions outside this module
void WriteIns(int8_t ins);
void WriteData(int8_t data);

uint8_t CheckBusy(void);
void IIC_put(uint8_t cb, uint8_t db);
static uint8_t IIC_get(void);

#endif	/* DOG104_H */

