/*
 * hts221.c
 *
 *  Created on: 29. 12. 2021
 *      Author: samue
 */

#include "hts221.h"
#include "math.h"

uint8_t HTSaddres = HTS221_DEVICE_ADDRESS;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, HTSaddres, 0));
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, HTSaddres, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, HTSaddres, 0);
}



uint8_t hts221_get_hum()
{
	uint8_t temp[2];
	hts221_readArray(temp, HTS221_ADDRESS_HUM_L, 2);

	return (((uint16_t)((temp[1] << 8) | temp[0])) >> 3);
}



uint8_t hts221_init(void)
{



uint8_t status = 1;



//LIS3MDL_ACC_ON;



LL_mDelay(100);



uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);



if(val == HTS221_WHO_AM_I_VALUE)
{
status = 1;
}
else //if the device is not found on one address, try another one
{
status = 0;
//return status;
}



//acc device init



uint8_t ctrl1 = 8 << 4; // +-2g res
hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);



return status;
}

