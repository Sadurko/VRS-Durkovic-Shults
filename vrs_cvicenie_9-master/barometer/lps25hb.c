/*
 * lps25hb.c
 *
 *  Created on: 29. 12. 2021
 *      Author: samue
 */

#include "lps25hb.h"
#include "math.h"

uint8_t LPSaddres = LPS25HB_DEVICE_ADDRESS_0;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, LPSaddres, 0));
}


void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, LPSaddres, 0);
}


void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, LPSaddres, 1);
}


float lps25hb_get_temp()
{
uint8_t temp[2];
lps25hb_readArray(temp, LPS25HB_ADDRESS_TEMP_L, 2);
float result = (((uint16_t)((temp[1] << 8) | temp[0])) >> 4) / 1000.0f + 25;
return roundf(result * 10) / 10;
}


float lps25hb_get_press()
{
uint8_t temp[3];
int32_t result = 0;
float output = 0;
lps25hb_readArray(temp, LPS25HB_ADDRESS_PRESS_L, 3);

result = result | ((uint32_t)(temp[0]));
result = result | ((uint32_t)(temp[1] << 8));
result = result | ((uint32_t)(temp[2] << 16));

output = result / 4096.0f;

return roundf(output * 100)/100;
}


float lps25hb_get_alt()
{
	uint8_t temp[2];
	lps25hb_readArray(temp, LPS25HB_ADDRESS_PRESS_L, 2);

	float P0 = 1013.25f;
	float P = (((uint16_t)((temp[1] << 8) | temp[0])) >> 4) / 4096.0f;
	float T = (((uint16_t)((temp[1] << 8) | temp[0])) >> 4) / 1000.0f + 25;
	float result = ((pow((P0/P), 0.190223) - 1) * (T + 273.15))/0.0065 ;

	return result;
}


float lps25hb_get_hum()
{
uint8_t temp[2];
lps25hb_readArray(temp, HTS221_ADDRESS_HUM_L , 2);
float result = (((uint16_t)((temp[1] << 8) | temp[0])) >> 4) / 1000.0f;
return roundf(result * 10) / 10;
}

uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		LPSaddres = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	//acc device init

	uint8_t ctrl1 = 8 << 4; // +-2g res
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}

