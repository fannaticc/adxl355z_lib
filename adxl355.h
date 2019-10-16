/* 
 * File:   adxl355.h
 * Author: raphael.pereira
 *
 * Created on 29 de Abril de 2019, 10:53
 */

#ifndef ADXL355_H
#define	ADXL355_H

#include <stdint.h>

//#define ADXL355_ADR 0x3A // ASEL = 0
#define ADXL355_ADR 0xA6 // ASEL = 1

#define TEMP2  0x06
#define TEMP1  0x07
#define XDATA3 0x08
#define XDATA2 0x09
#define XDATA1 0x0A
#define YDATA3 0x0B
#define YDATA2 0x0C
#define YDATA1 0x0D
#define ZDATA3 0x0E
#define ZDATA2 0x0F
#define ZDATA1 0x10

#define RANGE 0x2C
#define POWER_CTL 0x2D

#define RANGE_2G 0x01
#define RANGE_4G 0x02
#define RANGE_8G 0x03

#define MEASURE_MODE 0x04
#define OFF_MODE 0x01

#define ADXL355_TEMP_BIAS           (float)1852.0
#define ADXL355_TEMP_SLOPE          (float)-9.05


struct axis_value
{
    int32_t x_axis;
    int32_t y_axis;
    int32_t z_axis;
    int16_t temp_reg;
    float temp;
};

struct g_value
{
    float x_axis;
    float y_axis;
    float z_axis;
};

struct degrees
{
    float x_axis;
    float y_axis;
    float z_axis;
};

uint8_t axis_address[] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x06, 0x07};
// List of memory address.

void ADXL355_Init(uint8_t _range, uint8_t _power_mode);
struct axis_value ADXL355_Read_Axis(uint8_t address);
uint8_t I2C_Read_Byte(uint8_t i2c_address, uint8_t mem_address);
void I2C_Write_Byte(uint8_t i2c_address, uint8_t mem_address, uint8_t content);

int32_t ADXL355_Acceleration_Data_Conversion (uint32_t ui32SensorData);
int32_t ADXL355_twosCompliment(uint32_t value);
struct g_value ADXL355_Convert_G(struct axis_value raw_value);
struct degrees ADXL355_Convert_degree(struct g_value gravity);


#endif	/* ADXL355_H */

