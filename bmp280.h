/*
 * bmp280.h
 *
 *  Created on: Jul 1, 2024
 *      Author: kubaw
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "stm32f1xx_hal.h" //for hardware handling


int BMP280_init(uint8_t standby, uint8_t filter, uint8_t osrs_t, uint8_t osrs_p, uint8_t mode);
int32_t BMP280_MEASURE(void);
void reset(void);
void trimSettings(void);
int8_t trimData(void);
int32_t compensateTemp32(uint32_t rawT);
int32_t compensatePres32(uint32_t rawP, double t_fine1);



//general defines
#define BMP280_DEVICE_ADDRESS 0x76<<1 //adres is 0x76 but in 8 bits adress is moved to left by 1 to make space read/write bit
#define BMP280_CHIP_ID 0x58

//defines for data sectores
#define BMP280_TEMPERATURE_SETTINGS_START_ADDRESS 	0x88
#define BMP280_PRESSURE_SETTINGS_START_ADDRESS 		0x8E
#define BMP280_DATA_START_ADDRESS 					0xF7
#define BMP280_CONTROL_START_ADDRESS 				0xF4
#define BMP280_CONFIG_START_ADDRESS 				0xF5

//parameters defines
// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_SLEEP      0x00
#define MODE_FORCED     0x01
#define MODE_NORMAL     0x03

// Standby Time
#define T_SB_0p5    	0x00
#define T_SB_62p5   	0x01
#define T_SB_125    	0x02
#define T_SB_250    	0x03
#define T_SB_500    	0x04
#define T_SB_1000   	0x05
#define T_SB_10     	0x06
#define T_SB_20     	0x07

// IIR Filter Coefficients
#define IIR_OFF     	0x00
#define IIR_2       	0x01
#define IIR_4       	0x02
#define IIR_8       	0x03
#define IIR_16      	0x04




#endif /* INC_BMP280_H_ */
