/* 
 * File:   main.c
 * Author: raphael.pereira
 *
 * Created on 29 de Abril de 2019, 10:49
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "config.h"
#include "main.h"
#include "my_delays.h"
#include "my_i2c.h"
#include "serial.h"
#include "adxl355.h"
#include "ES15007.h"
#include "dog104.h"

/*
 * 
 */
void main(void)
{
    unsigned char aux[20];
    uint16_t serialn = 0;
    float temperature = 0;
    struct axis_value acc_value;
    struct degrees acc_angulos;
    struct g_value acc_g;
    float x = 0;
    float y = 0;
    float z = 0;
    
    OSCCON = 0b01110010;

    ADCON1 = 0b00001111; // Set all pins in digital mode.
    CMCON = 0b00000111; // Disable all comparators.

    TRISA = 0b00000000; // Configure PORTA as output
    TRISB = 0b11111111;
    TRISC = 0b10111001;
    
    // Inicialize serial port
    TXSTA = 0b00100110;
    RCSTA = 0b10010000;
    BAUDCON = 0b00001010;
    SPBRG = 16;  // 115200 @ 8MHz
    //SPBRG = 103; // 19200  @ 8MHz
    //SPBRG = 207; // 9600   @ 8MHz
    
    I2C_Close(); // Close the  I2C Bus.
    //I2C_Init(49); // I2C 100kHz, 20MHz-CRYSTAL.
    I2C_Init(19); // I2C 100kHz, 8MHz-CRYSTAL.
    //I2C_Init(9); // I2C 100kHz, 4MHz-CRYSTAL.
    __delay_ms(50);
    
    ADXL355_Init(RANGE_2G, MEASURE_MODE);
    initDispl(); //init display
    DisplayOnOff(DISPLAY_ON); //Disable cursor
    DisplLines(DOUBLEHEIGHT_OFF);
    ES15007_Init_Norm(); // This routine do nothing.
    
    putrsserial("ADXL355Z sensor tester\n");
    
    while(1)
    {
        acc_value = ADXL355_Read_Axis(ADXL355_ADR);
        
        acc_g = ADXL355_Convert_G(acc_value);
        
        sprintf(aux, "X: %+08.5fG  ", acc_g.x_axis);
        putrsserial(aux);
        sprintf(aux, "Y: %+08.5fG  ", acc_g.y_axis);
        putrsserial(aux);
        sprintf(aux, "Z: %+08.5fG  ", acc_g.z_axis);
        putrsserial(aux);
        
        //acc_angulos = ADXL355_Convert_degree(acc_g);
        
        /*sprintf(aux, "D_X: %7.5f°  ", acc_angulos.x_axis);
        putrsserial(aux);
        sprintf(aux, "D_Y: %7.5f°  ", acc_angulos.y_axis);
        putrsserial(aux);
        sprintf(aux, "D_Z: %7.5f°  ", acc_angulos.z_axis);
        putrsserial(aux);*/
        
        sprintf(aux, "Temp: %06.2f°C\n", acc_value.temp);
        putrsserial(aux);
        
        sprintf(aux, "X%+07.4fG", acc_g.x_axis);
        SetPostion(LINE1);
        WriteString(aux);
        
        sprintf(aux, "Y%+07.4fG", acc_g.y_axis);
        SetPostion(LINE2);
        WriteString(aux);
        
        sprintf(aux, "Z%+07.4fG", acc_g.z_axis);
        SetPostion(LINE3);
        WriteString(aux);
        
        sprintf(aux, "T:+%+6.2fC", acc_value.temp);
        SetPostion(LINE4);
        WriteString(aux);
        
        Delay_ms(1);
    }
}