/*
 * bmp280.c
 *
 *  Created on: Jul 1, 2024
 *      Author: kubaw
 */

#include "bmp280.h" //including header file, function prototypes

extern I2C_HandleTypeDef hi2c1; //enable i2c in this file
#define BMP280_I2C &hi2c1 //define which i2c is used for bmp280

uint8_t trimParameters[24]; //array for settings
uint8_t trimTP[6]; //array for raw temperature, pressure
int32_t rawTemperature;
int32_t rawPressure;
int32_t temperature1 = 0; //global variable for temprature (temperature in float format = temperature / 100)
int32_t pressure1 = 0; //global variable for pressure pressure in float format = pressure / 100;

//variables for checking and vriting data
uint8_t settings1;
uint8_t checkData1 = 0;
uint8_t settings2;
uint8_t checkData2 = 0;
double globalT_fine = 0;

uint16_t dig_T1, dig_P1;//variables for parameters in chip

int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

int BMP280_init(uint8_t standby, uint8_t filter, uint8_t osrs_t, uint8_t osrs_p, uint8_t mode)
{
	reset();

	settings2 = (standby<<5)|(filter<<2)|0x00; //test 00
	HAL_I2C_Mem_Write(BMP280_I2C, BMP280_DEVICE_ADDRESS, BMP280_CONFIG_START_ADDRESS, 1, &settings2, 1, 100);
	//check data
	HAL_I2C_Mem_Read(BMP280_I2C, BMP280_DEVICE_ADDRESS|0x01, BMP280_CONFIG_START_ADDRESS, 1, &checkData2, 1, 100);
	if(settings2 != checkData2)
	{
		return -1;
	}

	settings1 =  (osrs_t<<5)|(osrs_p<<2)|mode;
	HAL_I2C_Mem_Write(BMP280_I2C, BMP280_DEVICE_ADDRESS, BMP280_CONTROL_START_ADDRESS, 1, &settings1, 1, 100);
	//check data
	HAL_I2C_Mem_Read(BMP280_I2C, BMP280_DEVICE_ADDRESS|0x01, BMP280_CONTROL_START_ADDRESS, 1, &checkData1, 1, 100);
	if(settings1 != checkData1)
	{
		return -1;
	}



	trimSettings();


	return 0;
}
int32_t BMP280_MEASURE(void)
{
	trimData();
	compensateTemp32(rawTemperature);
	compensatePres32(rawPressure, globalT_fine);
	return 1;
}
void reset(void)
{
	uint8_t reset = 0xB6;
	HAL_I2C_Mem_Write(&hi2c1, BMP280_DEVICE_ADDRESS|0x00, 0xE0, 1, &reset, 1, 100);
}

void trimSettings(void) //data for equation
{
	HAL_I2C_Mem_Read(&hi2c1, BMP280_DEVICE_ADDRESS|0x01, BMP280_TEMPERATURE_SETTINGS_START_ADDRESS, 1, trimParameters, 24, 50);
	//parameters for temperature
	dig_T1 = (trimParameters[1]<<8) | trimParameters[0];

	dig_T2 = (trimParameters[3]<<8) | trimParameters[2];

	dig_T3 = (trimParameters[5]<<8) | trimParameters[4];

	dig_P1 = (trimParameters[7]<<8) | trimParameters[6];

	dig_P2 = (trimParameters[9]<<8) | trimParameters[8];

	dig_P3 = (trimParameters[11]<<8) | trimParameters[10];

	dig_P4 = (trimParameters[13]<<8) | trimParameters[12];

	dig_P5 = (trimParameters[15]<<8) | trimParameters[14];

	dig_P6 = (trimParameters[17]<<8) | trimParameters[16];

	dig_P7 = (trimParameters[19]<<8) | trimParameters[18];

	dig_P8 = (trimParameters[21]<<8) | trimParameters[20];

	dig_P9 = (trimParameters[23]<<8) | trimParameters[22];

}
int8_t trimData(void)
{
	HAL_I2C_Mem_Read(&hi2c1, BMP280_DEVICE_ADDRESS, BMP280_DATA_START_ADDRESS , 1, trimTP, 6, 100);

	rawPressure = (trimTP[0]<<12)|(trimTP[1]<<4)|(trimTP[2]>>4);
	rawTemperature = (trimTP[3]<<12)|(trimTP[4]<<4)|(trimTP[5]>>4);

	return 1;
}

int32_t compensateTemp32(uint32_t rawT)
{
	double var1, var2, t_fine;
	int32_t temperature;
	var1 = (((rawT/16384.0) - (dig_T1/1024.0)) * dig_T2);
	var2 = (((rawT)/131072.0 - (dig_T1)/8192.0) * ((rawT)/131072.0 - (dig_T1)/8192.0)) * (dig_T3);
	t_fine = var1 + var2;
	globalT_fine = t_fine;
	temperature = t_fine/51.20;
	temperature1 = temperature;
	return temperature;
}

int32_t compensatePres32(uint32_t rawP, double t_fine1)
{
	double var3, var4;
	int32_t pressure;
    var3 = (t_fine1/2.0) - 64000.0;
    var4 = var3 * var3 *(dig_P6/32768.0);
    var4 = var4 + (var3 * dig_P5 * 2.0);
    var4 = (var4/4.0) + (dig_P4 * 65536.0);
    var3 = ((dig_P3 * var3 * var3/523288.0) + (dig_P2 * var3)) / 524288.0;
    var3 = (1.0 + (var3/32768)) * dig_P1;
    pressure = 1048576.0 - rawPressure;
    pressure = (pressure - (var4/4096.0)) * (6250.0/var3);
    var3 = dig_P9 * pressure * (pressure/2147483648.0);
    var4 = pressure * (dig_P8/32768.0);
    pressure = pressure + (var3 + var4 + dig_P7)/16.0;
    pressure1 = pressure;
	return 1;
}
