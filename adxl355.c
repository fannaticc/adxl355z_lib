/* 
 * File:   adxl355.c
 * Author: raphael.pereira
 *
 * Created on 29 de Abril de 2019, 10:53
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "config.h"
#include "adxl355.h"
#include "my_i2c.h"
#include "serial.h"


void ADXL355_Init(uint8_t _range, uint8_t _power_mode)
{
    I2C_Write_Byte(ADXL355_ADR, RANGE, _range);
    I2C_Write_Byte(ADXL355_ADR, POWER_CTL, _power_mode);
}


struct axis_value ADXL355_Read_Axis(uint8_t address)
{
    struct axis_value axis;
    uint8_t buffer[15];
    uint8_t i = 0;
    
    I2C_Start();
    I2C_WriteByte(address); // Send the ADR to the talk device.
    I2C_WriteByte(TEMP2); // first memory address
    I2C_RepeatedStart();
    I2C_WriteByte(address + 1); // Send the ADR to the talk device.
    
    for(i = 0; i < 10; i++)
    {
        buffer[i] = I2C_ReadByte(1); // Read the data
    }
    
    buffer[++i] = I2C_ReadByte(0); // Read the data
    I2C_Stop();
    
    axis.x_axis = (uint32_t) buffer[2] << 24 | (uint32_t) buffer[3] << 16 | buffer[4] >> 8;
    axis.y_axis = (uint32_t) buffer[5] << 24 | (uint32_t) buffer[6] << 16 | buffer[7] >> 8;
    axis.z_axis = (uint32_t) buffer[8] << 24 | (uint32_t) buffer[9] << 16 | buffer[10] >> 8;

    axis.x_axis /= 4096;
    axis.y_axis /= 4096;
    axis.z_axis /= 4096;
    
    axis.temp_reg = (((uint16_t)buffer[0]) << 8) | (buffer[1]);
    axis.temp = ((axis.temp_reg - ADXL355_TEMP_BIAS)/ ADXL355_TEMP_SLOPE) + 25;
    
    return axis;
}

uint8_t I2C_Read_Byte(uint8_t i2c_address, uint8_t mem_address)
{
    uint8_t temp;
    I2C_Start();
    I2C_WriteByte(i2c_address); // Send the ADR to the talk device.
    I2C_WriteByte(mem_address);
    I2C_RepeatedStart();
    I2C_WriteByte(i2c_address + 1); // Send the ADR to the talk device.
    temp = I2C_ReadByte(0); // Read the data
    I2C_Stop();
    
    return temp;
}

void I2C_Write_Byte(uint8_t i2c_address, uint8_t mem_address, uint8_t content)
{
    I2C_Start();
    I2C_WriteByte(i2c_address); // Send the ADR to the talk device.
    I2C_WriteByte(mem_address); // Send the ADR to the talk device.
    I2C_WriteByte(content);
    I2C_Stop();
}

struct g_value ADXL355_Convert_G(struct axis_value raw_value)
{
    struct g_value g;
    
    g.x_axis = raw_value.x_axis / 256000.0;
    g.y_axis = raw_value.y_axis / 256000.0;
    g.z_axis = raw_value.z_axis / 256000.0;
    
    return g;
}

struct degrees ADXL355_Convert_degree(struct g_value gravity)
{
    struct degrees d;
    
    d.x_axis = (float)(1 / (float)sin(gravity.x_axis));
    d.y_axis = (float)(1 / (float)cos(gravity.y_axis));
    d.z_axis = (float)(1 / (float)tan(gravity.z_axis));
    
    /*d.x_axis = (double)(1 / tan(gravity.x_axis / (sqrt(pow(gravity.y_axis, 2)+pow(gravity.z_axis, 2)))));
    d.y_axis = (double)(1 / tan(gravity.y_axis / (sqrt(pow(gravity.x_axis, 2)+pow(gravity.z_axis, 2)))));
    d.z_axis = (double)(1 / tan((sqrt(pow(gravity.x_axis, 2)+pow(gravity.y_axis, 2))) / gravity.z_axis));*/
    
    return d;
}